/*
 * Copyright 2019, 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "vglite_support.h"
#include "semphr.h"
/*-----------------------------------------------------------*/
#include "vg_lite.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_BUFFER_COUNT 1
#define DEFAULT_SIZE     256.0f;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static int fb_width = 256, fb_height = 256;
static vg_lite_buffer_t fb[APP_BUFFER_COUNT];
static vg_lite_buffer_t dst_VG_buffer;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    PRINTF("hello vg world.\r\n");

    if (xTaskCreate(vglite_task, "vglite_task", configMINIMAL_STACK_SIZE + 800, NULL, configMAX_PRIORITIES - 1, NULL) !=
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

static void cleanup(void)
{
    for (uint8_t i = 0; i < APP_BUFFER_COUNT; i++)
    {
        if (fb[i].handle != NULL)
        {
            // Free the offscreen framebuffer memory.
            vg_lite_free(&fb[i]);
        }
    }

    vg_lite_close();
}

static void init_dst_vg_buffer(void)
{
    vg_lite_error_t error    = VG_LITE_SUCCESS;
    vg_lite_buffer_t *buffer = &dst_VG_buffer;

    // Allocate the off-screen buffer.
    buffer->width  = fb_width;
    buffer->height = fb_height;
    buffer->format = VG_LITE_RGB565;
    error          = vg_lite_allocate(buffer);
    if (error)
    {
        PRINTF("vg_lite_allocate() returned error %d\n", error);
        cleanup();
        return;
    }
    buffer->stride = fb_width * 2;
    buffer->width  = fb_width;
    buffer->height = fb_height;
    memset(buffer->memory, 0, buffer->stride * buffer->height);
}

static void run(void)
{
    vg_lite_error_t error  = VG_LITE_SUCCESS;
    vg_lite_filter_t filter = VG_LITE_FILTER_BI_LINEAR;
    char info[64];
    uint32_t chip_id;
    uint32_t chip_rev;
    int blit = 1;

    // Initialize the draw.
    error = vg_lite_init(fb_width, fb_height);
    if (error)
    {
        PRINTF("vg_lite engine init failed: vg_lite_init() returned error %d\n", error);
        cleanup();
        return;
    }

    vg_lite_get_product_info(info, &chip_id, &chip_rev);

    PRINTF("Framebuffer size: %d x %d\n", fb_width, fb_height);

    for (uint8_t i = 0; i < APP_BUFFER_COUNT; i++)
    {
        vg_lite_buffer_t *buffer = &fb[i];

        // Allocate the off-screen buffer.
        buffer->width  = fb_width;
        buffer->height = fb_height;
        buffer->format = VG_LITE_RGB565;
        error          = vg_lite_allocate(buffer);
        if (error)
        {
            PRINTF("vg_lite_allocate() returned error %d\n", error);
            cleanup();
            return;
        }

        buffer->stride = fb_width * 2;
        buffer->width  = fb_width;
        buffer->height = fb_height;

        /* Fill background with black. */
        memset(buffer->memory, 1, buffer->stride * buffer->height);
        PRINTF("before first color is %x\n", *(uint32_t *)buffer->memory);

        // Clear the buffer with blue.
        // error = vg_lite_clear(buffer, NULL, 0xFFFFFFFF);
        error = vg_lite_clear(buffer, NULL, 0xFFFF0000);

        vg_lite_finish();
        if (blit)
        {
            vg_lite_matrix_t matrix;
            vg_lite_identity(&matrix);
            init_dst_vg_buffer();
            vg_lite_blit(&dst_VG_buffer, buffer, &matrix, VG_LITE_BLEND_NONE, 0, filter);
            vg_lite_finish();
            PRINTF("The first dst color is %x\n", *(uint32_t *)dst_VG_buffer.memory);
        }
        PRINTF("The first color is %x\n", *(uint32_t *)buffer->memory);
    }
}

static void vglite_task(void *pvParameters)
{
    status_t status;

    status = BOARD_PrepareVGLiteController();

    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite contolor error\r\n");
        while (1)
            ;
    }

    run();

    cleanup();
}
