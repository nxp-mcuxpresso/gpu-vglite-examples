/*
 * Copyright 2019, 2021, 2024 NXP
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
#include "clock_analog.h"
#include "hour_needle.h"
#include "minute_needle.h"
#include "fsl_soc_src.h"


/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_UI_LAYERS (3)
#define ANALAOG_DIAL (0)
#define HOUR_NIDDLE (1)
#define MINUTE_NIDDLE (2)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;

static UILayers_t g_layers[MAX_UI_LAYERS] =
{
    UI_LAYER_DATA(ClockDial),
    UI_LAYER_DATA(HourNeedle),
    UI_LAYER_DATA(MinuteNeedle)
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_ResetDisplayMix(void)
{
    /*
     * Reset the displaymix, otherwise during debugging, the
     * debugger may not reset the display, then the behavior
     * is not right.
     */
    SRC_AssertSliceSoftwareReset(SRC, kSRC_DisplaySlice);
    while (kSRC_SliceResetInProcess == SRC_GetSliceResetState(SRC, kSRC_DisplaySlice))
    {
    }
}

int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    BOARD_ResetDisplayMix();
    BOARD_InitLpuartPins();
    BOARD_InitMipiPanelPins();
    BOARD_InitDebugConsole();

    if (xTaskCreate(vglite_task, "vglite_task", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES - 1, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

static vg_lite_error_t init_vg_lite(void)
{
    vg_lite_error_t error = VG_LITE_SUCCESS;

    error = VGLITE_CreateDisplay(&display);
    if (error)
    {
        PRINTF("VGLITE_CreateDisplay failed: VGLITE_CreateDisplay() returned error %d\r\n", error);
        return error;
    }
    // Initialize the window.
    error = VGLITE_CreateWindow(&display, &window);
    if (error)
    {
        PRINTF("VGLITE_CreateWindow failed: VGLITE_CreateWindow() returned error %d\r\n", error);
        return error;
    }
    // Initialize the draw.
    error = vg_lite_init(DEFAULT_VG_LITE_TW_WIDTH, DEFAULT_VG_LITE_TW_HEIGHT);
    if (error)
    {
        PRINTF("vg_lite engine init failed: vg_lite_init() returned error %d\r\n", error);
        vg_lite_close();
        return error;
    }
    // Set GPU command buffer size for this drawing task.
    error = vg_lite_set_command_buffer_size(VG_LITE_COMMAND_BUFFER_SIZE);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\r\n", error);
        vg_lite_close();
        return error;
    }

    return error;
}

uint32_t getTime()
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

void init() {
    int ret = 0;
    vg_lite_error_t error;
    for (int i = 0; i < MAX_UI_LAYERS; i++) {
        ret = layer_init(&g_layers[i]);
        if (ret != VG_LITE_SUCCESS) {
            PRINTF("\r\nERROR: layer_init failed for %s\r\n",g_layers[i].img_info->image_name);
        }
    }
    error = gradient_cache_init();
    if (error != 0)
    {
        PRINTF("init_cached_gradient failed: error %d\r\n", error);
        while (1)
            ;
    }
}

void draw_hour_niddle(vg_lite_buffer_t *rt, float angle) {

    vg_lite_matrix_t m;
    vg_lite_identity(&m);
    vg_lite_translate(200, 200, &m);
    vg_lite_rotate(angle, &m);

    layer_draw(rt, &g_layers[HOUR_NIDDLE], &m);

}

void draw_minute_niddle(vg_lite_buffer_t *rt, float angle) {

    vg_lite_matrix_t m;
    vg_lite_identity(&m);
    vg_lite_translate(200, 200, &m);
    vg_lite_rotate(angle, &m);

    layer_draw(rt, &g_layers[MINUTE_NIDDLE], &m);
}

void draw_analog_dial(vg_lite_buffer_t *rt) {

    vg_lite_matrix_t m;
    vg_lite_identity(&m);
    layer_draw(rt, &g_layers[ANALAOG_DIAL], &m);
}

void redraw(vg_lite_buffer_t *rt) {
	static float angle = 0;

    draw_analog_dial(rt);
    draw_hour_niddle(rt, angle);
    draw_minute_niddle(rt, -angle);
	angle += 0.5;
}


static void vglite_task(void *pvParameters)
{
    status_t status;
    vg_lite_error_t error;

    status = BOARD_PrepareVGLiteController();
    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite contolor error\r\n");
        while (1)
            ;
    }

    error = init_vg_lite();
    if (error)
    {
        PRINTF("init_vg_lite failed: init_vg_lite() returned error %d\r\n", error);
        while (1)
            ;
    }

    init();

    uint32_t startTime, time, n = 0;
	startTime = getTime();
	while(1)
	{
		vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);
		if (rt == NULL)
		{
			PRINTF("vg_lite_get_renderTarget error\r\n");
			while (1)
				;
		}

		vg_lite_clear(rt, NULL, 0x0);
		redraw(rt);

		VGLITE_SwapBuffers(&window);

		n++;
		if (n > 60)
		{
			time = getTime() - startTime;
			PRINTF("%d frames in %d seconds: %d fps\r\n", n, time / 1000, n * 1000 / time);
			n         = 0;
			startTime = getTime();
		}
	}
	for (int i = 0; i < MAX_UI_LAYERS; i++) {
		free_layer(&g_layers[i]);
	}
	gradient_cache_free();
}
