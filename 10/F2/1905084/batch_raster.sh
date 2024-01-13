#!/bin/bash
g++ raster.cpp -o raster 
mkdir -p outputs
for d in IOs/*/ ; do
    cp "${d}scene.txt" ./
    cp "${d}config.txt" ./
    mkdir -p "outputs/${d}"
    ./raster
    mv "stage1.txt" "outputs/${d}"
    mv "stage2.txt" "outputs/${d}"
    mv "stage3.txt" "outputs/${d}"
    mv "z_buffer.txt" "outputs/${d}"
    mv "out.bmp" "outputs/${d}"
    rm scene.txt
    rm config.txt
done