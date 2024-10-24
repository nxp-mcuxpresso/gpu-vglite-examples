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
   ./gpu-vglite-toolkit.sh ${ASSET_PATH}/$1 ${OUTPUT_PATH}/$2

   # Include layer.h in generated header file
   include_layer_h ${OUTPUT_PATH}/$2
}

echo Clock dial options
echo 0. Dial with Orange color
echo 1. Dial with linear grardient
echo 2. Dial with Radial grardient
echo 9. TO exit
read -p "Select dial: [1-3]:" dial
echo 

echo Hour needle options
echo 0. Needle with fixed color
echo 9. TO exit
read -p "Select needle: [0]:" hour 
echo 

echo Minute needle options
echo 0. Needle with fixed color
echo 1. Needle with stroke
echo 9. TO exit
read -p "Select needle: [1-2]:" minute
echo 

clock_dial_options=("ClockAnalogOrange.svg" "ClockAnalogLinear.svg" "ClockAnalogRadial.svg")
hour_needle_options=("HourNeedle.svg")
minute_needle_options=("MinuteNeedle.svg" "MinuteNeedleStroke.svg")

CLOCK_DIAL_SVG=${clock_dial_options[$dial]}
HOUR_DIAL_SVG=${hour_needle_options[$hour]}
MINUTE_DIAL_SVG=${minute_needle_options[$minute]}

echo ${dial} ${minute}
echo Dial: ${CLOCK_DIAL_SVG}
echo Hour: ${HOUR_DIAL_SVG}
echo Minute: ${MINUTE_DIAL_SVG}

transform_svg_to_header ${CLOCK_DIAL_SVG} clock_analog.h
transform_svg_to_header ${HOUR_DIAL_SVG} hour_needle.h
transform_svg_to_header ${MINUTE_DIAL_SVG} minute_needle.h

