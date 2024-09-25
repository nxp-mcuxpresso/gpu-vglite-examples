#!/bin/bash

# This script builds tiger_freertos, vector_freertos and tiled_freertos applications for a certain board

set -e

if [[ $# -ne 1 ]]; then
    echo "Usage: build_board.sh <board_name>" >&2
    exit 2
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
elif [ "$1" = "evkmimxrt1170" ] || [ "$1" = "evkmimxrt1160" ] || [ "$1" = "evkbmimxrt1170" ]; then
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

./build_script.sh  examples/src/vglite_examples/tiger_freertos tiger_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core}
./build_script.sh  examples/src/vglite_examples/vector_freertos vector_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core}
./build_script.sh  examples/src/vglite_examples/tiled_freertos tiled_freertos ${deb} ${rel} ${flash_deb} ${flash_rel} $1 ${core}