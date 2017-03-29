#!/bin/bash

function trace {
    echo $1 ; date +"%m/%d/%Y %H:%M:%S" ; echo ""
}

# Load the configuration
source config.sh

cd "$benchmark_dir"

for function in ${functions[@]}; do
    trace "# Function: $function"

    # Index the base images
    trace "Index: Base"
    ./"$ris_app" -f "$function" index "$benchmark_base_dir"
    # Index the transformed images
    for t in ${transformations[@]}; do
        trace "Index: ${transformation_name[$t]}"
        ./"$ris_app" -f "$function" index "$t"
    done

    # Search and compute the measures.
    trace "Search:"
    (
        ./"$ris_app" -f "$function" search "$benchmark_base_dir" -t ${end_threshold[$function]};
        for t in ${transformations[@]}; do
            ./"$ris_app" -f "$function" search "$t" -t ${end_threshold[$function]};
        done;
        ./"$ris_app" -f "$function" search "$benchmark_non_indexed_dir" -t ${end_threshold[$function]}
    ) | ../RisBenchmarkResult --relevant_items $relevant_items --start_threshold ${start_threshold[$function]} --step_threshold ${step_threshold[$function]} --end_threshold ${end_threshold[$function]} > "result_$function.dat"
    
    gnuplot "../threshold_$function.pg" > "threshold_$function.png"
    rm index
done

# Plot results
trace "Plot results"
gnuplot "../roc.pg" > "roc.png"

trace "END"
