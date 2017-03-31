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

# Possible values: "DCT" "MH" "RV"
declare -a functions=("DCT" "MH" "RV")
# Possible values: "blur" "gray" "half" "compress10" "rotate5" "crop10"
declare -a transformations=("blur" "gray" "half" "compress10" "rotate5" "crop10")

declare -A transformation_name
transformation_name["blur"]="Blur"
transformation_name["gray"]="Grayscale"
transformation_name["half"]="Half size"
transformation_name["compress10"]="Compress with quality 10"
transformation_name["rotate5"]="Rotation with angle 5"
transformation_name["crop10"]="Crop right 10%"

declare -A transformation_command
transformation_command["blur"]="mogrify -path blur -blur 4x2 $benchmark_base_dir/*"
transformation_command["gray"]="mogrify -path gray -type Grayscale $benchmark_base_dir/*"
transformation_command["half"]="mogrify -path half -resize 50% $benchmark_base_dir/*"
transformation_command["compress10"]="mogrify -path compress10 -format jpg -quality 10 $benchmark_base_dir/*"
transformation_command["rotate5"]="mogrify -path rotate5 -rotate 5 $benchmark_base_dir/*"
transformation_command["crop10"]="mogrify -path crop10 -crop 90%x100%+0+0 $benchmark_base_dir/*"

relevant_items=$((${#transformations[@]} + 1))