#!/bin/bash
export CXX=clang++-5.0
export CC=clang-5.0
$CXX --version
cmake --version
conan --version
mkdir Build
cd Build
conan remote add yage https://api.bintray.com/conan/bentoudev/yage
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install ..
cmake --target all -DCMAKE_BUILD_TYPE=Release -DYAGE_VERSION_PATCH=$PATCH_VER ..
make