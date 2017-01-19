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
$ cmake -DCMAKE_BUILD_TYPE=Release .
$ make

# Build the project with CMake optimised for a SSE4 CPU.
$ cmake -DCMAKE_BUILD_TYPE=Release -DWITH_NATIVE_ARCH=ON -DWITH_POPCNT=ON .
$ make
```
The executable is then located in the same directory.

Usage
-----

### 
```bash
# Display the help
$ ./pHashRis -h

# Index the content of a directory
$ ./pHashRis index <dir_path>

# Display statistics about the indexed images
$ ./pHashRis statistics

# Display all image pairs whose distance is less than or equal to 0.16
$ ./pHashRis distances -t 0.16

# Search for an image
$ ./pHashRis search <file_path>

# Search for all the images in a directory
$ ./pHashRis search <directory_path>
```

License
-------
See the LICENSE file.