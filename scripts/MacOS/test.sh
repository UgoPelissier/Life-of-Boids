#! /bin/bash

cd ../../build
ctest
test -f src/life-of-boids

echo "[INFO] Tests were successful"