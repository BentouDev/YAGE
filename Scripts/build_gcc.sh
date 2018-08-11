#!/bin/bash
export CXX=g++
export CC=gcc
$CXX --version
cmake --version
conan --version
mkdir Build
cd Build
conan remote add yage https://api.bintray.com/conan/bentoudev/yage
conan install ..
cmake --target all -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage " -DYAGE_BUILD_TESTS=On -DENABLE_COVERAGE=On -DCMAKE_BUILD_TYPE=Release -DYAGE_VERSION_PATCH=$PATCH_VER ..
make