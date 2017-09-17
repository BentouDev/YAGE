#!/bin/bash
export CXX=g++-6
export CC=gcc-6
$CXX --version
cmake --version
mkdir bin
cd bin
cmake -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage " -DENABLE_COVERAGE=On --target all ..
make