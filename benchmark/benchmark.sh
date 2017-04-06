#!/bin/bash

function trace {
    echo $1 ; date +"%m/%d/%Y %H:%M:%S" ; echo ""
}

# Load the configuration
source config.sh

cd "$benchmark_dir"

for function in ${functions[@]}; do
    trace "# Function: $function"

    # Index only the base images
    trace "Index: Base"
    ./"$ris_app" -f "$function" index "$benchmark_base_dir"

    # Benchmark on individual transformations
    if [ "$individual_transformation_benchmark" = true ] ; then
        # Search and compute the measures for individual transformations
        trace "Benchmark individual transformations:"
        # Non-indexed
        (
            ./"$ris_app" -f "$function" search "$benchmark_non_indexed_dir" -t ${end_threshold[$function]}
        ) | ../RisBenchmarkResult --relevant_items 1 --start_threshold ${start_threshold[$function]} --step_threshold ${step_threshold[$function]} --end_threshold ${end_threshold[$function]} > "result_${function}_non-indexed.dat"
        gnuplot -e "filename='result_${function}_non-indexed.dat';name='${function} (non-indexed)'" "../threshold.pg" > "threshold_${function}_non-indexed.png"
        # Bases
        (
            ./"$ris_app" -f "$function" search "$benchmark_base_dir" -t ${end_threshold[$function]}
        ) | ../RisBenchmarkResult --relevant_items 1 --start_threshold ${start_threshold[$function]} --step_threshold ${step_threshold[$function]} --end_threshold ${end_threshold[$function]} > "result_${function}_base.dat"
        gnuplot -e "filename='result_${function}_base.dat';name='${function} (base)'" "../threshold.pg" > "threshold_${function}_base.png"
        # Transformations
        for t in ${transformations[@]}; do
            (
                ./"$ris_app" -f "$function" search "$t" -t ${end_threshold[$function]}
            ) | ../RisBenchmarkResult --relevant_items 1 --start_threshold ${start_threshold[$function]} --step_threshold ${step_threshold[$function]} --end_threshold ${end_threshold[$function]} > "result_${function}_$t.dat"
            gnuplot -e "filename='result_${function}_$t.dat';name='${function} ($t)'" "../threshold.pg" > "threshold_${function}_$t.png"
        done
    fi

    # Benchmark on all modifications at the same time
    if [ "$multiple_transformation_benchmark" = true ] ; then
        # Index the transformed images
        for t in ${transformations[@]}; do
            trace "Index: ${transformation_name[$t]}"
            ./"$ris_app" -f "$function" index "$t"
        done


        # Search and compute the measures for all transformations
        (
            ./"$ris_app" -f "$function" search "$benchmark_base_dir" -t ${end_threshold[$function]};
            for t in ${transformations[@]}; do
                ./"$ris_app" -f "$function" search "$t" -t ${end_threshold[$function]};
            done;
            ./"$ris_app" -f "$function" search "$benchmark_non_indexed_dir" -t ${end_threshold[$function]}
        ) | ../RisBenchmarkResult --relevant_items $relevant_items --start_threshold ${start_threshold[$function]} --step_threshold ${step_threshold[$function]} --end_threshold ${end_threshold[$function]} > "result_$function.dat"
        gnuplot -e "filename='result_$function.dat';name='${function}'" "../threshold.pg" > "threshold_$function.png"
    fi

    # Remove index file
    rm index
done

if [ "$multiple_transformation_benchmark" = true ] ; then
    # Plot results
    trace "Plot results"
    gnuplot "../roc.pg" > "roc.png"
fi

trace "END"
