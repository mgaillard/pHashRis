#include <iostream>
#include "pHash.h"
#include "DCTpHash.h"
#include "Hamming.h"

using namespace std;

DCTpHash DCTpHashFunction(const string &file_path) {
    ulong64 file_hash;

    if (ph_dct_imagehash(file_path.c_str(), file_hash) >= 0) {

    } else {
        cerr << "Error while calculating the hash of the file " << file_path << "." << endl;
    }

    return static_cast<DCTpHash>(file_hash);
}

double DCTpHashDistance(const DCTpHash &hash_a, const DCTpHash &hash_b) {
    return static_cast<double>(hamming_distance(hash_a, hash_b)) / 64;
}