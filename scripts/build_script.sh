#!/bin/bash

set -e

if [[ $# -ne 8 ]]; then
    echo "Usage: build_script.sh <example_folder> <application_name> <target_debug_name> <target_release_name> <target_flash_debug_name> <target_flash_release_name> <board_name> <core_name>" >&2
    exit 2
fi

if [ -d build_cmake ]; then
  rm -r build_cmake
fi

core=""
if [ "$1" != "none" ]; then
   core="-Dcore_id=$8"
fi

exec_name="$2_$8.elf"

exec_debug_armgcc="$2-armgcc-$3-$7.elf"
exec_release_armgcc="$2-armgcc-$4-$7.elf"
exec_flash_debug_armgcc="$2-armgcc-$5-$7.elf"
exec_flash_release_armgcc="$2-armgcc-$6-$7.elf"

exec_debug_iar="$2-iar-$3-$7.elf"
exec_release_iar="$2-iar-$4-$7.elf"
exec_flash_debug_iar="$2-iar-$5-$7.elf"
exec_flash_release_iar="$2-iar-$6-$7.elf"

echo -e "Environment:"
echo -e "binary name: ${exec_name}"
echo -e "board: $7"
echo -e "core: ${core}"
echo -e "armgcc binary debug name: ${exec_debug_armgcc}"
echo -e "armgcc binary release name: ${exec_release_armgcc}"
echo -e "armgcc binary flash debug name: ${exec_flash_debug_armgcc}"
echo -e "armgcc binary flash release name: ${exec_flash_release_armgcc}"
echo -e "iar binary debug name: ${exec_debug_iar}"
echo -e "iar binary release name: ${exec_release_iar}"
echo -e "iar binary flash debug name: ${exec_flash_debug_iar}"
echo -e "iar binary flash release name: ${exec_flash_release_iar}"

# cube_freertos only builds on iar debug and iar release targets
if [ "$2" != "cube_freertos" ]; then
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $3"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $3
  cp build_cmake/${exec_name} ${exec_debug_armgcc}
  rm -rf build_cmake
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $4"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $4
  cp build_cmake/${exec_name} ${exec_release_armgcc}
  rm -rf build_cmake
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $5"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $5
  cp build_cmake/${exec_name} ${exec_flash_debug_armgcc}
  rm -rf build_cmake
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $6"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain armgcc --config $6
  cp build_cmake/${exec_name} ${exec_flash_release_armgcc}
  rm -rf build_cmake
fi

echo -e ""
echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $3"
echo -e ""
west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $3
cp build_cmake/${exec_name} ${exec_debug_iar}
rm -rf build_cmake
echo -e ""
echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $4"
echo -e ""
west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $4
cp build_cmake/${exec_name} ${exec_release_iar}
rm -rf build_cmake

if [ "$2" != "cube_freertos" ]; then
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $5"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $5
  cp build_cmake/${exec_name} ${exec_flash_debug_iar}
  rm -rf build_cmake
  echo -e ""
  echo -e "Running command: west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $6"
  echo -e ""
  west build -b $7 $1 ${core} --build-dir=build_cmake --toolchain iar --config $6
  cp build_cmake/${exec_name} ${exec_flash_release_iar}
  rm -rf build_cmake
fi
