#! /bin/bash

# setup the build directory
rm -rf build
mkdir build
cd build

# start the build process
conan install .. --build=missing -c tools.system.package_manager:mode=install
MODE="${MODE:=Release}"
cmake -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -DCMAKE_BUILD_TYPE="${MODE}"   ..
cmake --build .

echo "[INFO] build was successful"