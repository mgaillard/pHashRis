#include <iostream>
#include "pHash.h"
#include "DCTHash.h"
#include "Hamming.h"

using namespace std;

uint64_t DCTHash(const string &file_path) {
    ulong64 file_hash;

    if (ph_dct_imagehash(file_path.c_str(), file_hash) >= 0) {

    } else {
        cerr << "Error while calculating the hash of the file " << file_path << "." << endl;
    }

    return static_cast<uint64_t>(file_hash);
}

double DCTHashDistance(uint64_t hash_a, uint64_t hash_b) {
    return static_cast<double>(hamming_distance(hash_a, hash_b)) / 64;
}