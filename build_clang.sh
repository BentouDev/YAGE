#!/bin/bash
export CXX=clang++-5.0
export CC=clang-5.0
mkdir bin
cd bin
cmake --target all ..
make