#!/bin/bash
export CXX=clang++-5.0
export CC=clang-5.0
$CXX --version
cmake --version
mkdir bin
cd bin
cmake --target all -DYAGE_VERSION_PATCH=$PATCH_VER ..
make