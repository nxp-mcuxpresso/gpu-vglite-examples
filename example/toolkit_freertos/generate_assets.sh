#!/bin/bash

TOOLKIT_PATH=/opt/NXP/GTEC/VGLITE_FINAL/gpu-vglite-toolkit
OUTPUT_PATH=${PWD}
ASSET_PATH=${OUTPUT_PATH}/resources

function include_layer_h() {
   echo -e "#include \"vglite_layer.h\"\n" > out.h
   cat $1 >> out.h  
   mv out.h $1
}

function transform_svg_to_header() {
   ${TOOLKIT_PATH}/gpu-vglite-toolkit.sh $1 ${OUTPUT_PATH}/$2

   # Include vglite_layer.h in generated header file
   include_layer_h ${OUTPUT_PATH}/$2
}

svg_file=("paint-stroke-01-t.svg")

function generate_assets() {
    SVG_FILE=$1

    echo chosen svg file: ${SVG_FILE}

    # Remaing files to avoid modification in toolkit_freertos.c
    cp -v ${ASSET_PATH}/${SVG_FILE} svgt12_test.svg

    transform_svg_to_header ${PWD}/svgt12_test.svg svgt12_test.h

    rm -v svgt12_test.svg
}

generate_assets ${svg_file}

