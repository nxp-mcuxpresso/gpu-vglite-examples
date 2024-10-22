/*
 * Copyright 2019, 2021, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "layer.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;

/*******************************************************************************
 * Code
 ******************************************************************************/

UILayers_t g_layers[MAX_UI_LAYERS] =
{
	{&ClockAnalogOrange, NULL, NULL, &ClockAnalogOrange_gradient_info, ClockAnalogOrange_color_data},
	{&HourNeedle, NULL, NULL, &HourNeedle_gradient_info, HourNeedle_color_data},
	{&MinuteNeedle, NULL, NULL, &MinuteNeedle_gradient_info, MinuteNeedle_color_data}
};

static void BOARD_ResetDisplayMix(void)
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
    for (int i = 0; i < MAX_UI_LAYERS; i++) {
        init_layer(&g_layers[i]);
    }
}

void draw_hour_niddle(vg_lite_buffer_t *rt, float angle) {
    for (int j = 0; j < g_layers[HOUR_NIDDLE].path->path_count; j++) {
        memcpy(&g_layers[HOUR_NIDDLE].matrix[j].m, &g_layers[HOUR_NIDDLE].path->transform[j * (sizeof(g_layers[HOUR_NIDDLE].matrix[j].m) / sizeof(float))], sizeof(g_layers[HOUR_NIDDLE].matrix[j].m));
		vg_lite_translate(200, 200, &g_layers[HOUR_NIDDLE].matrix[j]);
		vg_lite_rotate(angle, &g_layers[HOUR_NIDDLE].matrix[j]);
		redraw_layer(rt, &g_layers[HOUR_NIDDLE], &g_layers[HOUR_NIDDLE].matrix[j]);
    }
}

void draw_minute_niddle(vg_lite_buffer_t *rt, float angle) {
    for (int j = 0; j < g_layers[MINUTE_NIDDLE].path->path_count; j++) {
      	memcpy(g_layers[MINUTE_NIDDLE].matrix[j].m, &g_layers[MINUTE_NIDDLE].path->transform[j * (sizeof(g_layers[MINUTE_NIDDLE].matrix[j].m) / sizeof(float))], sizeof(g_layers[MINUTE_NIDDLE].matrix[j].m));
		vg_lite_translate(200, 200, &g_layers[MINUTE_NIDDLE].matrix[j]);
		vg_lite_rotate(angle, &g_layers[MINUTE_NIDDLE].matrix[j]);
		redraw_layer(rt, &g_layers[MINUTE_NIDDLE], &g_layers[MINUTE_NIDDLE].matrix[j]);
    }
}

void redraw(vg_lite_buffer_t *rt) {
	static float angle = 0;
	for (int j = 0; j < g_layers[ANALAOG_DIAL].path->path_count; j++) {
		memcpy(&g_layers[ANALAOG_DIAL].matrix[j].m, &g_layers[ANALAOG_DIAL].path->transform[j * (sizeof(g_layers[ANALAOG_DIAL].matrix[j].m) / sizeof(float))], sizeof(g_layers[ANALAOG_DIAL].matrix[j].m));
		redraw_layer(rt, &g_layers[ANALAOG_DIAL], &g_layers[ANALAOG_DIAL].matrix[j]);
		draw_hour_niddle(rt, angle);
		draw_minute_niddle(rt, angle);
	}
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
	while (1)
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
}
