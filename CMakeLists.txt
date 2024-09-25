
if(CONFIG_MCUX_COMPONENT_middleware.vglite)
    mcux_add_source(
        SOURCES inc/vg_lite.h
                VGLite/vg_lite.c
                VGLite/vg_lite_image.c
                VGLite/vg_lite_matrix.c
                VGLite/vg_lite_path.c
                VGLite/vg_lite_stroke.c
                VGLite/vg_lite_options.h
                VGLite/vg_lite_context.h
                VGLite/rtos/vg_lite_os.c
                VGLite/Series/gc555/0x423_ECO/vg_lite_options.h
                VGLite/Series/gc355/0x0_1216/vg_lite_options.h
                VGLite/Series/gc255/0x40A/vg_lite_options.h
                VGLiteKernel/vg_lite_hal.h
                VGLiteKernel/vg_lite_hw.h
                VGLiteKernel/vg_lite_kernel.c
                VGLiteKernel/vg_lite_kernel.h
                VGLiteKernel/vg_lite_option.h
                VGLiteKernel/vg_lite_type.h
                VGLiteKernel/vg_lite_debug.h
                VGLiteKernel/rtos/vg_lite_hal.c
                VGLiteKernel/rtos/vg_lite_platform.h
                RELEASE_NOTES.txt
                LICENSE.txt
        BASE_PATH ${SdkRootDirPath}/middleware/vglite/driver/
    )
    mcux_add_include(
        INCLUDES inc
                 VGLite
                 VGLite/rtos
                 VGLite/Series/gc555/0x423_ECO
                 VGLite/Series/gc355/0x0_1216
                 VGLite/Series/gc255/0x40A
                 VGLiteKernel
                 VGLiteKernel/rtos
        BASE_PATH ${SdkRootDirPath}/middleware/vglite/driver/
    )
endif()
