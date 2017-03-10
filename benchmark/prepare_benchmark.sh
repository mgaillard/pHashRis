#!/bin/bash

function trace {
    echo $1 ; date +"%m/%d/%Y %H:%M:%S" ; echo ""
}

# Load the configuration
source config.sh

# Copy files from the benchmark
trace "Preparation"
cp RisBenchmarkResult "$benchmark_dir"
cp threshold_DCT.pg "$benchmark_dir"
cp threshold_MH.pg "$benchmark_dir"
cp threshold_RV.pg "$benchmark_dir"
cp roc.pg "$benchmark_dir"
cp "$ris_path/$ris_app" "$benchmark_dir"

# Create directories for the images
mkdir "$benchmark_dir/$benchmark_base_dir"
mkdir "$benchmark_dir/$benchmark_non_indexed_dir"
mkdir "$benchmark_dir/$benchmark_blur_dir"
mkdir "$benchmark_dir/$benchmark_gray_dir"
mkdir "$benchmark_dir/$benchmark_half_dir"
mkdir "$benchmark_dir/$benchmark_compress10_dir"
mkdir "$benchmark_dir/$benchmark_rotate5_dir"
mkdir "$benchmark_dir/$benchmark_crop10_dir"

# Split and copy the images in the benchmark folder
trace "Base image copy"
count=0
ls "$image_dir" | sort | while read file; do
    if [ $count -lt $image_test_number ]; then
        cp "$image_dir/$file" "$benchmark_dir/$benchmark_non_indexed_dir"
    else
        cp "$image_dir/$file" "$benchmark_dir/$benchmark_base_dir"
    fi
    (( count++ ))
done

cd $benchmark_dir
# Transform the images
trace "Transformation: Blur"
mogrify -path $benchmark_blur_dir -blur 4x2 $benchmark_base_dir/*
trace "Transformation: Grayscale"
mogrify -path $benchmark_gray_dir -type Grayscale $benchmark_base_dir/*
trace "Transformation: Half size"
mogrify -path $benchmark_half_dir -resize 50% $benchmark_base_dir/*
trace "Transformation: Compress with quality 10"
mogrify -path $benchmark_compress10_dir -format jpg -quality 10 $benchmark_base_dir/*
trace "Transformation: Rotation with angle 5"
mogrify -path $benchmark_rotate5_dir -rotate 5 $benchmark_base_dir/*
trace "Transformation: Crop right 10%"
mogrify -path $benchmark_crop10_dir -crop 90%x100%+0+0 $benchmark_base_dir/*