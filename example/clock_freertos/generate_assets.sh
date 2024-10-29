#!/bin/bash

TOOLKIT_PATH=/opt/BUILD/GTEC/VGLITE/gpu-vglite-toolkit
OUTPUT_PATH=/opt/BUILD/GTEC/VGLITE/gpu-vglite-examples/example/clock_freertos
ASSET_PATH=${OUTPUT_PATH}/resources

function include_layer_h() {
   echo -e "#include \"layer.h\"\n" > out.h  
   cat $1 >> out.h  
   mv out.h $1
}

function transform_svg_to_header() {
   cd ${TOOLKIT_PATH}
   ./gpu-vglite-toolkit.sh $1 ${OUTPUT_PATH}/$2

   # Include layer.h in generated header file
   include_layer_h ${OUTPUT_PATH}/$2
   cd -
}

clock_dial_options=("ClockAnalogOrange.svg" "ClockAnalogLinear.svg" "ClockAnalogRadial.svg")
hour_needle_options=("HourNeedle.svg")
minute_needle_options=("MinuteNeedle.svg" "MinuteNeedleStroke.svg")


function generarte_clock() {
    CLOCK_DIAL_SVG=$1
    HOUR_DIAL_SVG=$2
    MINUTE_DIAL_SVG=$3

    echo Dial: ${CLOCK_DIAL_SVG}
    echo Hour: ${HOUR_DIAL_SVG}
    echo Minute: ${MINUTE_DIAL_SVG}

    # Remaing files to avoid modification in clock_freertos.c
    cp -v ${ASSET_PATH}/${CLOCK_DIAL_SVG} ClockDial.svg
    cp -v ${ASSET_PATH}/${MINUTE_DIAL_SVG} MinuteNeedle.svg

    transform_svg_to_header ${ASSET_PATH}/${HOUR_DIAL_SVG} hour_needle.h
    transform_svg_to_header ${PWD}/ClockDial.svg clock_analog.h
    transform_svg_to_header ${PWD}/MinuteNeedle.svg minute_needle.h

    rm -v ClockDial.svg
    rm -v MinuteNeedle.svg
}

# Orange Dial, fixed hour needle, fixed minute needle
function basic_clock() {
    generarte_clock ${clock_dial_options[0]} ${hour_needle_options[0]} ${minute_needle_options[0]}
}

# Dial with Linear Graadient, fixed hour needle, stroked based minute needle
function advanced_clock() {
    generarte_clock ${clock_dial_options[1]} ${hour_needle_options[0]} ${minute_needle_options[1]}
}

function show_usage() {
    echo To generate Basic clock
    echo "$0 basic"
    echo 

    echo To generate Advanced clock
    echo "$0 advanced"
    echo 
}

[ "$1" = "" ] && show_usage $0
[ "$1" = "basic" ] && basic_clock
[ "$1" = "advanced" ] && advanced_clock

