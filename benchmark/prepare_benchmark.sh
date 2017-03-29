#!/bin/bash

function trace {
    echo $1 ; date +"%m/%d/%Y %H:%M:%S" ; echo ""
}

# Load the configuration
source config.sh

# Copy files from the benchmark
trace "Preparation"
cp "$ris_path/$ris_app" "$benchmark_dir"

# Create directories for the images
mkdir "$benchmark_dir/$benchmark_base_dir"
mkdir "$benchmark_dir/$benchmark_non_indexed_dir"
for t in ${transformations[@]}; do
    mkdir "$benchmark_dir/$t"
done

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
for t in ${transformations[@]}; do
    trace "Transformation: ${transformation_name[$t]}"
    eval ${transformation_command[$t]}
done