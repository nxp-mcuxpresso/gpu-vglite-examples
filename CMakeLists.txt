
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

    mcux_add_iar_configuration(
        CX "--diag_suppress=Pe257,Pa039"
        CC "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
        CX "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
    )

    mcux_add_armgcc_configuration(
        CC "-fms-extensions" "-Wno-unused-variable"
        CX "-fms-extensions" "-Wno-unused-variable"
        CC  "-fms-extensions            \
            -Wno-unused-variable        \
            -Wno-unused-function        \
            -Wno-unused-parameter       \
            -Wno-enum-conversion        \
            -Wno-int-conversion         \
            -Wno-sign-compare"
        CX  "-fms-extensions            \
            -Wno-unused-variable        \
            -Wno-unused-function        \
            -Wno-unused-parameter       \
            -Wno-enum-conversion        \
            -Wno-sign-compare"
    )

    mcux_add_mdk_configuration(
        CC "-fms-extensions" "--diag_suppress=1296"
        CX "-fms-extensions" "--diag_suppress=1296"
        CC "-fms-extensions" "--diag_suppress=1296,257,39,188,89,191,111,93,177,546,236,205,136,84" "-Wno-tautological-constant-out-of-range-compare"
        CX "-fms-extensions" "--diag_suppress=1296,257,39,188,89,191,111,93,177,546,236,205,136,84" "-Wno-tautological-constant-out-of-range-compare"
    )
endif()
