#!/bin/bash
export CXX=g++
export CC=gcc
$CXX --version
cmake --version
mkdir bin
cd bin
cmake -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage " -DENABLE_COVERAGE=On --target all ..
make