pHashRis
=======
pHashRis is a reverse image search engine based on pHash. This software is developed as part of a specific student project at INSA Lyon, France.  

Features
--------

- Index the images from a directory
- Search for images even with slightly modified versions
- Display statistics about the indexed images
- Display the distances between the indexed images

How it works ?
--------------
pHashRis makes use of pHash in order to extract a perceptual hash of the images. A distance is then computed between the query image and the indexed images. The most relevant image is the nearest to the query image.

Installation
----------------

You first need to clone the project on your computer, and build it with one of the following commands.

```bash
# Build the project with CMake
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../src
$ make

# Build the project with CMake optimised for a SSE4 CPU.
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DWITH_NATIVE_ARCH=ON -DWITH_POPCNT=ON ../src
$ make
```
The executable is then located in the build directory.

Usage
-----

```bash
# Display the help
$ ./pHashRis -h

# Index the content of a directory (by default using a DCT based hash)
$ ./pHashRis index <dir_path>

# Display statistics about the indexed images
$ ./pHashRis statistics

# Display all image pairs whose distance is less than or equal to 0.16
$ ./pHashRis distances -t 0.16

# Search for an image
$ ./pHashRis search <file_path>

# Search for all the images in a directory
$ ./pHashRis search <directory_path>

# Index the content of a directory using a MH based hash
$ ./pHashRis -f MH index <directory_path>

# Search for all the images in a directory using a MH based hash
$ ./pHashRis -f MH search <directory_path>
```

Benchmark
---------
A benchmark is implemented along with pHashRis in order to measure the effectiveness of the different perceptual hash functions. The benchmark runs on Ubuntu 16.04 LTS.

```bash
# Run the benchmark
$ cd benchmark
$ bash run.sh
```

License
-------
See the LICENSE file.