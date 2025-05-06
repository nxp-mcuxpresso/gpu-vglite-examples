#!/bin/bash

# This script builds tiger_freertos, cube_freertos, vector_freertos and tiled_freertos applications for a certain board

set -e

if [[ $# -ne 3 ]]; then
    echo "Usage: build_board.sh <board_name> <examples_folder> <toolchain_selection>"
    echo "examples_folder: path/to/examples/vglite_examples, it can be a relative path"
    echo "toolchain_selection: 'iar', 'armgcc', 'mdk', 'all'"
    exit 1
fi

echo -e "Environment:"
echo -e "board name: $1"
echo -e ""

core=""
deb=""
rel=""
flash_deb=""
flash_rel=""
if [ "$1" = "evkmimxrt595" ]; then
    core="cm33"
    deb="debug"
    rel="release"
    flash_deb="flash_debug"
    flash_rel="flash_release"
elif [ "$1" = "evkmimxrt1160" ] || [ "$1" = "evkbmimxrt1170" ]; then
    core="cm7"
    deb="sdram_debug"
    rel="sdram_release"
    flash_deb="flexspi_nor_sdram_debug"
    flash_rel="flexspi_nor_sdram_release"
elif [ "$1" = "mimxrt700evk" ]; then
    core="cm33_core0"
    deb="debug"
    rel="release"
    flash_deb="flash_debug"
    flash_rel="flash_release"
fi

examples_path_tiger_freertos="$2/tiger_freertos"
examples_path_vector_freertos="$2/vector_freertos"
examples_path_tiled_freertos="$2/tiled_freertos"
examples_path_clock_freertos="$2/cube_freertos"
examples_path_cube_freertos="$2/clock_freertos"
examples_path_toolkit_freertos="$2/toolkit_freertos"
examples_path_decompress_etc2_freertos="$2/decompress_etc2_freertos"

./build_script.sh  ${examples_path_tiger_freertos} tiger_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
./build_script.sh  ${examples_path_vector_freertos} vector_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
./build_script.sh  ${examples_path_tiled_freertos} tiled_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
./build_script.sh  ${examples_path_clock_freertos} cube_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
./build_script.sh  ${examples_path_cube_freertos} clock_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
./build_script.sh  ${examples_path_toolkit_freertos} toolkit_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
if [ "$1" = "mimxrt700evk" ]; then
  ./build_script.sh  ${examples_path_decompress_etc2_freertos} decompress_etc2_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core} $3
fi