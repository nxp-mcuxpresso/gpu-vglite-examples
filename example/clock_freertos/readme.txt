generate_assets.sh is a helper script which will let you generate clock_freertos graphics assets quickly.

PRE-REQUSITE
============
  Graphics assets are generatd using gpu-vglite-toolkit
  Clone gpu-vglite-toolkit from github
      mkdir /opt/nxp
      cd /opt/nxp
      git clone https://github.com/nxp-mcuxpresso/gpu-vglite-toolkit
      export TOOLKIT_PATH=/opt/nxp/gpu-vglite-toolkit

  In nut-shell you need to run following command to generate graphics assets
      ${TOOLKIT_PATH}/gpu-vglite-toolkit.sh ClockAnalogOrange.svg clock_analog.h

QUICK STEPS
===========
  To generate a clock basic dial with fixed color hour needle and minute needle.
  ./generate_assets.sh basic

  To generate a clock with linear gradient based dial with fixed color hour needle and minute needle.
  ./generate_assets.sh advanced

Steps to utilize manually generated graphics assets
===================================================
  In case you generate graphics assets manually.
    ${TOOLKIT_PATH}/gpu-vglite-toolkit.sh ClockAnalogOrange.svg clock_analog.h

  Each graphics assets header contains image_info structure, with variable name as input-svg file name.
  e.g.
    static image_info_t ClockAnalogOrange = {
        .image_name ="ClockDial",
        .image_size = {400, 400},

  Each such image_info is treated as layer. This layer can be initialized simpliy in clock_freertos.c
in global layer array g_layers.
   UI_LAYER_DATA(ClockAnalogOrange),


NOTE:
  1. You will need to set TOOLKIT_PATH and OUTPUT_PATH correctly in generate_assets.sh properly.
  2. When UI_LAYER_DATA is passed wrong image_info_t variable it results in build error.

