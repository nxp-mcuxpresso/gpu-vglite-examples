#!/bin/bash

set -e

# runs all binaries in this folder using
# a gdb macro provided as a argument

for i in ./*.elf; do
    # Process $i
    echo -e $i
    arm-none-eabi-gdb -x $1 $i
done