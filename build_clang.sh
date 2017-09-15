#!/bin/bash
export CXX=clang++-3.7
export CC=clang-3.7
mkdir bin
cd bin
cmake --target all ..
make