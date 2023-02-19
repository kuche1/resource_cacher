#! /usr/bin/env bash

set -e
set -o xtrace

rm -rf build
mkdir build
cd build
clear
cmake -S .. -B .
make
RESOURCE_CACHER_DATA_DIR=/tmp/cache ./test
