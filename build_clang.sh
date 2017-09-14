#!/bin/bash
export CXX=clang++-3.7
export CC=clang-3.7
cd bin
cmake --target all ..
make