pHashRis
=======
pHashRis is a reverse image search engine based on pHash. This software is developed as part of a specific student project at INSA Lyon, France.  

Features
--------

- Index the images from a directory
- Search for images even with slightly modified versions
- Display the distances between the indexed images

How it works ?
--------------
pHashRis makes use of pHash in order to extract a perceptual hash of the images. A distance is then computed between the query image and the indexed images. The most relevant image is the nearest to the query image.

Installation
----------------

You first need to clone the project on your computer, and build it with the following command.

```bash
# Build the project with CMake
$ cmake .
$ make
```
The executable is then located in the same directory.

Usage
-----

### 
```bash
# Index the content of a directory
$ ./pHashRis index <dir_path>

# Search for an image
$ ./pHashRis search <file_path>

# Search for all the images in a directory
$ ./pHashRis search <directory_path>

# Display all image pairs whose distance is less than or equal to 16
$ ./pHashRis distances 16
```

License
-------
See the LICENSE file.