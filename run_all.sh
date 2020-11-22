#!/usr/bin/env bash

num_samples=$1

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j || exit 1
cd ..

# Run all testcases.
# You can comment some lines to disable the run of specific examples.
mkdir -p output
bin/BINART 01_earth_in_box $num_samples
