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
cmake -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage " -DENABLE_COVERAGE=On -DYAGE_VERSION_PATCH=$PATCH_VER --target all ..
make