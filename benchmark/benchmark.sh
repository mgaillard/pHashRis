#!/bin/bash

function trace {
    echo $1 ; date +"%m/%d/%Y %H:%M:%S" ; echo ""
}

# Load the configuration
source config.sh

cd "$benchmark_dir"

for function in "DCT" "MH" "RV"; do
    # Index the images
    trace "Index: Base"
    ./"$ris_app" -f "$function" index "$benchmark_base_dir"
    trace "Index: Blur"
    ./"$ris_app" -f "$function" index "$benchmark_blur_dir"
    trace "Index: Grayscale"
    ./"$ris_app" -f "$function" index "$benchmark_gray_dir"
    trace "Index: Half size"
    ./"$ris_app" -f "$function" index "$benchmark_half_dir"
    trace "Index: Compress with quality 10"
    ./"$ris_app" -f "$function" index "$benchmark_compress10_dir"
    trace "Index: Rotation with angle 5"
    ./"$ris_app" -f "$function" index "$benchmark_rotate5_dir"
    trace "Index: Crop right 15%"
    ./"$ris_app" -f "$function" index "$benchmark_crop10_dir"

    # Search and compute the measures.
    trace "Search:"
    (
        ./"$ris_app" -f "$function" search "$benchmark_non_indexed_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_base_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_blur_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_gray_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_half_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_compress10_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_rotate5_dir" -t ${end_threshold[${function}]};
        ./"$ris_app" -f "$function" search "$benchmark_crop10_dir" -t ${end_threshold[${function}]}
    ) | ./RisBenchmarkResult --relevant_items 7 --start_threshold ${start_threshold[${function}]} --step_threshold ${step_threshold[${function}]} --end_threshold ${end_threshold[${function}]} > "result_$function.dat"
    
    gnuplot "threshold_$function.pg" > "threshold_$function.png"
    rm index
done

# Plot results
trace "Plot results"
gnuplot roc.pg > roc.png

trace "END"
