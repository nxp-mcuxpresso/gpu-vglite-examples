generate_assets.sh is a helper script which will let you generate toolkit_freertos graphics assets quickly.

PRE-REQUSITE
============
  Graphics assets are generatd using gpu-vglite-toolkit
  Clone gpu-vglite-toolkit from github
      mkdir /opt/nxp
      cd /opt/nxp
      git clone https://github.com/nxp-mcuxpresso/gpu-vglite-toolkit
      export TOOLKIT_PATH=/opt/nxp/gpu-vglite-toolkit

  In nut-shell you need to run following command to generate graphics assets
      ${TOOLKIT_PATH}/gpu-vglite-toolkit.sh example_file.svg svgt12_test.h

QUICK STEPS
===========
  To generate graphic assets from toolkit_freertos, create a 'resources' folder and place the desired SVG file inside it.
  ./generate_assets.sh example_file.svg


Steps to utilize manually generated graphics assets
===================================================
  In case you generate graphics assets manually.
    ${TOOLKIT_PATH}/gpu-vglite-toolkit.sh example_file.svg example_file.h

  Each graphics assets header contains image_info structure, with variable name as input-svg file name.
  e.g.
    static image_info_t example_file = {
        .image_name ="example_file",
        .image_size = {400, 400},

  Each such image_info is treated as layer. This layer can be initialized simpliy in toolkit_freertos.c
in global layer array g_layers.
   UI_LAYER_DATA(example_file),


NOTE:
  1. You will need to set TOOLKIT_PATH and OUTPUT_PATH correctly in generate_assets.sh properly.
  2. When UI_LAYER_DATA is passed wrong image_info_t variable it results in build error.
