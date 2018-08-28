#!/bin/bash

# Install dependencies
sudo apt install -y build-essential cimg-dev libsndfile1 libsndfile1-dev libsamplerate0 libsamplerate0-dev libmpg123-0 libmpg123-dev git libboost-all-dev cmake unzip imagemagick gnuplot

# Download MIRFLICKR25000
wget press.liacs.nl/mirflickr/mirflickr25k.v2/mirflickr25k.zip
unzip mirflickr25k.zip
rm mirflickr25k.zip
cd mirflickr
rm -rf doc
rm -rf meta
cd ..

# Install pHash
wget www.phash.org/releases/pHash-0.9.6.tar.gz
tar -zxvf pHash-0.9.6.tar.gz
rm pHash-0.9.6.tar.gz
cd pHash-0.9.6
./configure
make -j4
sudo make install
sudo ldconfig
cd ..
rm -rf pHash-0.9.6

# Compile pHashRis
mkdir pHashRis
cd pHashRis
cmake -DCMAKE_BUILD_TYPE=Release -DWITH_NATIVE_ARCH=ON -DWITH_POPCNT=ON ../../src
make -j4
cd ..

# Prepare the benchmark
cd Result
cmake -DCMAKE_BUILD_TYPE=Release .
make -j2
cp RisBenchmarkResult ..
cd ..

mkdir benchmark
