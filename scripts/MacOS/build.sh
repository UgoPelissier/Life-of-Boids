#! /bin/bash

# setup the build directory
cd ../..
rm -rf build
mkdir build
cd build

# start the build process
conan install .. --build=missing
cmake -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake -DCMAKE_BUILD_TYPE="${MODE}" ..
cmake --build .

echo "[INFO] Build was successful"