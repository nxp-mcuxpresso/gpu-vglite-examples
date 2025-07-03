/*
 * Copyright 2019, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VGLITE_WINDOW_H_
#define _VGLITE_WINDOW_H_

#include "fsl_common.h"
#include "vg_lite.h"
#include "vglite_support.h"
#include "display_support.h"
#include "fsl_fbdev.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(MIMXRT798S_cm33_core0_SERIES) &&                                    \
    ((DEMO_PANEL == DEMO_PANEL_RK055AHD091 &&                                   \
      DEMO_RK055AHD091_USE_XRGB8888 == 1) ||                                    \
     (DEMO_PANEL == DEMO_PANEL_RK055MHD091 &&                                   \
      DEMO_RK055AHD091_USE_XRGB8888 == 1))
#define USE_BUFFER_COMPRESSION 1
#endif

#define APP_BUFFER_COUNT 2

typedef struct vg_lite_compression_info
{
    uint16_t strideBytes;
    video_pixel_format_t pixelFormat;
    vg_lite_compress_mode_t compressionMode;
} vg_lite_compression_info_t;

typedef struct vg_lite_display
{
    fbdev_t g_fbdev;
    fbdev_fb_info_t g_fbInfo;
} vg_lite_display_t;

typedef struct vg_lite_window
{
    vg_lite_display_t *display;
    vg_lite_buffer_t buffers[APP_BUFFER_COUNT];
    int width;
    int height;
    int bufferCount;
    int current;
} vg_lite_window_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

vg_lite_error_t VGLITE_CreateDisplay(vg_lite_display_t *display);

vg_lite_error_t VGLITE_CreateWindow(vg_lite_display_t *display,
                                    vg_lite_window_t *window,
                                    vg_lite_compression_info_t *c_info);

vg_lite_error_t VGLITE_DestoryWindow(void);

vg_lite_error_t VGLITE_DestroyDisplay(void);

vg_lite_buffer_t *VGLITE_GetRenderTarget(vg_lite_window_t *window);

void VGLITE_SwapBuffers(vg_lite_window_t *window);

vg_lite_error_t VGLITE_SetCompressionInfo(vg_lite_compression_info_t *c_info);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _VGLITE_WINDOW_H_ */
