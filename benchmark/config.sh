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
benchmark_blur_dir="blur"
benchmark_gray_dir="gray"
benchmark_half_dir="half"
benchmark_compress10_dir="compress10"
benchmark_rotate5_dir="rotate5"
benchmark_crop10_dir="crop10"