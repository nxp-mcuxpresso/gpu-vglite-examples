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

/* Prototypes */
void* vg_lite_os_malloc(uint32_t size);
void vg_lite_os_free(void *memory);


//NOTE: This API(multiply) is from vg_lite_matrix.c
static void multiply(vg_lite_matrix_t * matrix, vg_lite_matrix_t * mult)
{
    vg_lite_matrix_t temp;
    int row, column;

    /* Process all rows. */
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

int layer_draw(vg_lite_buffer_t *rt, UILayers_t *layer, vg_lite_matrix_t *transform_matrix)
{
	vg_lite_error_t error;
	const int matrix_size = sizeof(vg_lite_matrix_t);
	const int matrix_size_in_float = (sizeof(vg_lite_matrix_t) / sizeof(float));

    if (rt == NULL || layer == NULL || transform_matrix == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    for (int i = 0; i < layer->path->path_count; i++) {
		if((layer->mode->hybridPath[2 * i].fillType == FILL_CONSTANT) &&
				(layer->mode->hybridPath[2 * i].pathType == VG_LITE_DRAW_FILL_PATH)){

            vg_lite_matrix_t *tmatrix = (vg_lite_matrix_t *)&layer->matrix[i].m;

            memcpy(tmatrix,
                &layer->path->transform[i * matrix_size_in_float],
				matrix_size);

            multiply(tmatrix, transform_matrix);

			error = vg_lite_draw(rt, &layer->handle[i], layer->mode->fillRule[i], transform_matrix, VG_LITE_BLEND_NONE, layer->color[i]);
			if (error) {
				PRINTF("Error: vg_lite_draw() returned error %d\r\n", error);
				return error;
			}
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


