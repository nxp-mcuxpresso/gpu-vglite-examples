/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#include "layer.h"

/* GLobals */
static gradient_cache_entry_t *g_grad_cache = NULL;

/* Prototypes */
void* vg_lite_os_malloc(uint32_t size);
void vg_lite_os_free(void *memory);

//NOTE: This API(multiply) is from vg_lite_matrix.c
static void multiply(vg_lite_matrix_t * matrix, vg_lite_matrix_t * mult)
{
    vg_lite_matrix_t temp;
    int row, column;

    for (row = 0; row < 3; row++) {
        /* Process all columns. */
        for (column = 0; column < 3; column++) {
            /* Compute matrix entry. */
            temp.m[row][column] =  (matrix->m[row][0] * mult->m[0][column])
            + (matrix->m[row][1] * mult->m[1][column])
            + (matrix->m[row][2] * mult->m[2][column]);
        }
    }

    /* Copy temporary matrix into result. */
#if VG_SW_BLIT_PRECISION_OPT
    memcpy(matrix, &temp, sizeof(vg_lite_float_t) * 9);
#else
    memcpy(matrix, &temp, sizeof(temp));
#endif /* VG_SW_BLIT_PRECISION_OPT */
}

int gradient_cache_init(void)
{
    g_grad_cache = (gradient_cache_entry_t *)vg_lite_os_malloc(MAX_GRADIENT_CACHE * sizeof(gradient_cache_entry_t));
    if (g_grad_cache == NULL) {
        PRINTF("Error: Memory allocation failed for g_grad_cache!\n");
        return VG_LITE_OUT_OF_MEMORY;
    }
    memset(g_grad_cache, 0, MAX_GRADIENT_CACHE * sizeof(g_grad_cache));
    return VG_LITE_SUCCESS;
}

void gradient_cache_free(void)
{
    int i;
    if (g_grad_cache != NULL) {
        for (i = 0; i < MAX_GRADIENT_CACHE; i++) {
            if (g_grad_cache[i].g != NULL) {
                if (g_grad_cache[i].type == eLinearGradientCacheEntry) {
                    vg_lite_clear_linear_grad(&g_grad_cache[i].grad_data.lg.lGradient);
                }
            }
        }
        vg_lite_os_free(g_grad_cache);
        g_grad_cache = NULL;
    }
}

void _gradient_stop_color_to_vglite_color(int32_t num_stop_points, stopValue_t *stops, vg_lite_color_ramp_t *vgColorRamp)
{
	int i;

	for (i = 0; i < num_stop_points && i < MAX_GRADIENT_STOP_POINTS; i++) {
		vgColorRamp[i].stop = stops[i].offset;
		vgColorRamp[i].red = ((stops[i].stop_color & 0x00FF0000) >> 16) / 255.0f;
		vgColorRamp[i].green = ((stops[i].stop_color & 0x0000FF00) >> 8) / 255.0f;
		vgColorRamp[i].blue = (stops[i].stop_color & 0x000000FF) / 255.0f;
		vgColorRamp[i].alpha = ((stops[i].stop_color & 0xFF000000) >> 24) / 255.0f;
	}
}

int gradient_cache_find(void *grad, int type, gradient_cache_entry_t **ppcachedEntry)
{
	int unused_idx;
    int i;
    vg_lite_error_t error;
    gradient_cache_entry_t *cachedGradient = NULL;

    /* Reset output pointer to NULL by default, indicating cache search failed. */
    *ppcachedEntry = NULL;

    if (grad == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    if (! (type == eLinearGradientCacheEntry))
        return VG_LITE_INVALID_ARGUMENT;

    /* Check if path object for given gradient exists */
	for (i=0; i<MAX_GRADIENT_CACHE; i++) {
		cachedGradient = &g_grad_cache[i];
		if (cachedGradient->g == grad)
		{
			*ppcachedEntry = cachedGradient;
			return VG_LITE_SUCCESS;
		}
	}

	/* Find un-used descriptor */
	unused_idx = -1;
	for (i=1; i<MAX_GRADIENT_CACHE; i++) {
		cachedGradient = &g_grad_cache[i];
		if ( cachedGradient->g == NULL ) {
			unused_idx = i;
		}
	}
	if (unused_idx == -1) {
		return NULL;
	}

	cachedGradient = &g_grad_cache[unused_idx];
	/* Release memory of last gradient */
	if(cachedGradient->type == eLinearGradientCacheEntry){
		vg_lite_clear_linear_grad(&cachedGradient->grad_data.lg.lGradient);
	}

	/* Allocate and cache requested gradient descriptor */
	if(type == eLinearGradientCacheEntry){
		linearGradient_t *gradient = (linearGradient_t *)grad;

		_gradient_stop_color_to_vglite_color(
				gradient->num_stop_points,
				gradient->stops,
				cachedGradient->vgColorRamp);

		cachedGradient->grad_data.lg.lGradient.count = gradient->num_stop_points;
		cachedGradient->grad_data.lg.polygonLinearGradient = gradient->linear_gradient;
		error = vg_lite_set_linear_grad(&cachedGradient->grad_data.lg.lGradient,
				cachedGradient->grad_data.lg.lGradient.count,
				cachedGradient->vgColorRamp,
				cachedGradient->grad_data.lg.polygonLinearGradient,
				VG_LITE_GRADIENT_SPREAD_PAD, 1);
		if (error != VG_LITE_SUCCESS)
			return error;
		error = vg_lite_update_linear_grad(&cachedGradient->grad_data.lg.lGradient);

	}

	if (error != VG_LITE_SUCCESS)
		return error;

	cachedGradient->g = grad;
	cachedGradient->type = type;

	*ppcachedEntry = cachedGradient;
	return VG_LITE_SUCCESS;
}

int layer_draw(vg_lite_buffer_t *rt, UILayers_t *layer, vg_lite_matrix_t *transform_matrix)
{
	vg_lite_error_t error;
	static vg_lite_color_ramp_t vgColorRamp[256];
	int type;

	const int matrix_size = sizeof(vg_lite_matrix_t);
	const int matrix_size_in_float = (sizeof(vg_lite_matrix_t) / sizeof(float));
	gradient_cache_entry_t *cachedGradient = NULL;

    if (rt == NULL || layer == NULL || transform_matrix == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    for (int i = 0; i < layer->path->path_count; i++) {
		vg_lite_matrix_t *tmatrix = (vg_lite_matrix_t *)&layer->matrix[i].m;

		memcpy(tmatrix,
			&layer->path->transform[i * matrix_size_in_float],
			matrix_size);

		multiply(tmatrix, transform_matrix);
		switch (layer->mode->hybridPath[2 * i].fillType) {
		case STROKE:
		case FILL_CONSTANT:
			error = vg_lite_draw(rt, &layer->handle[i], layer->mode->fillRule[i], transform_matrix, VG_LITE_BLEND_NONE, layer->color[i]);
			if (error) {
				PRINTF("Error: vg_lite_draw() returned error %d\r\n", error);
				return error;
			}
			break;
		case FILL_LINEAR_GRAD:
			error = gradient_cache_find(layer->mode->linearGrads[i],
					eLinearGradientCacheEntry,
					&cachedGradient);
			if (error != VG_LITE_SUCCESS)
				continue;
			cachedGradient->grad_data.lg.lGradient.matrix = *transform_matrix;
			if (cachedGradient == NULL) {
				PRINTF("Error: Failed to get cached linear gradient. Please increase MAX_GRADIENT_CACHE.\n");
				return VG_LITE_OUT_OF_MEMORY;
			}

			error = vg_lite_draw_linear_gradient(rt, &layer->handle[i],
					layer->mode->fillRule[i], transform_matrix,
					&cachedGradient->grad_data.lg.lGradient, 0, VG_LITE_BLEND_NONE, VG_LITE_FILTER_LINEAR);
			if (error != VG_LITE_SUCCESS)
				return error;
			break;
		case FILL_RADIAL_GRAD:
			error = gradient_cache_find(layer->mode->radialGrads[i],
					eRadialGradientCacheEntry,
					&cachedGradient);
			if (error != VG_LITE_SUCCESS)
				return error;
			cachedGradient->grad_data.rg.rGradient.matrix = *transform_matrix;
			if (cachedGradient == NULL) {
				PRINTF("Error: Failed to get cached radial gradient. Please increase MAX_GRADIENT_CACHE.\n");
				return VG_LITE_OUT_OF_MEMORY;
			}

			error = vg_lite_draw_radial_gradient(rt, &layer->handle[i],
					layer->mode->fillRule[i], transform_matrix,
					&cachedGradient->grad_data.rg.rGradient, 0, VG_LITE_BLEND_NONE, VG_LITE_FILTER_LINEAR);
			if (error != VG_LITE_SUCCESS)
				return error;
			break;
		default:
			return VG_LITE_INVALID_ARGUMENT;
		}
    }
    vg_lite_finish();
    return VG_LITE_SUCCESS;
}

int layer_init(UILayers_t *layer)
{
    int i;
    vg_lite_error_t vg_err;

    if (layer == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    layer->handle  = (vg_lite_path_t *)vg_lite_os_malloc(layer->path->path_count * sizeof(vg_lite_path_t));
    if (layer->handle == NULL) {
		PRINTF("\r\nERROR: Memory allocation failed for path!\r\n\r\n");
		return VG_LITE_OUT_OF_MEMORY;
	}
    memset(layer->handle, 0 , layer->path->path_count * sizeof(vg_lite_path_t));
    layer->matrix  = (vg_lite_matrix_t *)vg_lite_os_malloc(layer->path->path_count * sizeof(vg_lite_matrix_t));
    if (layer->matrix == NULL) {
		PRINTF("\r\nERROR: Memory allocation failed for matrix!\r\n\r\n");
		return VG_LITE_OUT_OF_MEMORY;
	}
    for (i = 0; i < layer->path->path_count; i++) {
        vg_err = vg_lite_init_path(&layer->handle[i], layer->path->data_format, VG_LITE_MEDIUM,
        		layer->path->paths_info[i].path_length, layer->path->paths_info[i].path_data,
            0, 0, layer->path->image_size[0], layer->path->image_size[1]);
        if (vg_err != VG_LITE_SUCCESS) {
		    PRINTF("\r\nERROR: Failed to initialize graphic artifacts!\r\n\r\n");
		    return vg_err;
	    }
        layer->handle[i].add_end = layer->path->paths_info[i].end_path_flag;
	}

	return VG_LITE_SUCCESS;
}

int layer_free(UILayers_t *layer)
{
    if (layer == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    if (layer->handle) {
        vg_lite_os_free(layer->handle);
        layer->handle = NULL;
    }
    if (layer->matrix) {
        vg_lite_os_free(layer->matrix);
        layer->matrix = NULL;
    }

	return VG_LITE_SUCCESS;
}


