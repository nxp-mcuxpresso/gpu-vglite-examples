#!/bin/bash

set -e

if [[ $# -ne 1 ]]; then
    echo "Usage: build_all_boards.sh <examples_folder>"
    echo "examples_folder: path/to/examples/vglite_examples, it can be a relative path"
    exit 1
fi

./build_board.sh evkbmimxrt1170 $1 all
./build_board.sh evkmimxrt1160 $1 all
./build_board.sh mimxrt700evk $1 all
./build_board.sh evkmimxrt595 $1 all
