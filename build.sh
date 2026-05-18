#!/bin/bash

clear
mkdir -p build
cd build
rm -rf *
cmake ../ -DDEBUG_BUILD=ON
make -j4
