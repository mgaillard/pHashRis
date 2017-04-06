ris_app="pHashRis"
ris_path="pHashRis"
image_dir="mirflickr"
image_test_number=1000

declare -A start_threshold
start_threshold["DCT"]=0
start_threshold["MH"]=0
start_threshold["RV"]=0

declare -A step_threshold
step_threshold["DCT"]=0.03125
step_threshold["MH"]=0.005
step_threshold["RV"]=0.005

declare -A end_threshold
end_threshold["DCT"]=0.34375
end_threshold["MH"]=0.45
end_threshold["RV"]=0.17

benchmark_dir="benchmark"
benchmark_base_dir="base"
benchmark_non_indexed_dir="non-indexed"

# Functions to benchmark
# Possible values: "DCT" "MH" "RV"
declare -a functions=("DCT" "MH" "RV")

# Modifications to apply on images
# Possible values: "blur" "gray" "half" "compress10" "rotate1" "rotate2" "rotate3" "rotate4" "rotate5" "crop10"
declare -a transformations=("blur" "gray" "half" "compress10" "rotate5" "crop10")

# Set to "true" to perform a benchmark on individual transformations
individual_transformation_benchmark=true

# Set to "true" to performa a benchmark on all modifications at the same time
multiple_transformation_benchmark=true

declare -A transformation_name
transformation_name["blur"]="Blur"
transformation_name["gray"]="Grayscale"
transformation_name["half"]="Half size"
transformation_name["compress10"]="Compress with quality 10"
transformation_name["rotate1"]="Rotation with angle 1"
transformation_name["rotate2"]="Rotation with angle 2"
transformation_name["rotate3"]="Rotation with angle 3"
transformation_name["rotate4"]="Rotation with angle 4"
transformation_name["rotate5"]="Rotation with angle 5"
transformation_name["crop10"]="Crop right 10%"

declare -A transformation_command
transformation_command["blur"]="mogrify -path blur -blur 4x2 $benchmark_base_dir/*"
transformation_command["gray"]="mogrify -path gray -type Grayscale $benchmark_base_dir/*"
transformation_command["half"]="mogrify -path half -resize 50% $benchmark_base_dir/*"
transformation_command["compress10"]="mogrify -path compress10 -format jpg -quality 10 $benchmark_base_dir/*"
transformation_command["rotate1"]="mogrify -path rotate1 -rotate 1 $benchmark_base_dir/*"
transformation_command["rotate2"]="mogrify -path rotate2 -rotate 2 $benchmark_base_dir/*"
transformation_command["rotate3"]="mogrify -path rotate3 -rotate 3 $benchmark_base_dir/*"
transformation_command["rotate4"]="mogrify -path rotate4 -rotate 4 $benchmark_base_dir/*"
transformation_command["rotate5"]="mogrify -path rotate5 -rotate 5 $benchmark_base_dir/*"
transformation_command["crop10"]="mogrify -path crop10 -crop 90%x100%+0+0 $benchmark_base_dir/*"

relevant_items=$((${#transformations[@]} + 1))
