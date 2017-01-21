#ifndef PHASHRIS_DCTHASH_H
#define PHASHRIS_DCTHASH_H

#include <cstdint>
#include <string>

/**
 * A 64 bits hash.
 */
typedef uint64_t DCTpHash;

/**
 * Compute the DCT based image hash of an image.
 * @param file_path A path to an image.
 * @return A 64 bits image hash.
 */
DCTpHash DCTpHashFunction(const string &file_path);

/**
 * Compute the distance between two DCT based image hashes.
 * The Hamming distance is used.
 * @param hash_a A 64 bits image hash.
 * @param hash_b A 64 bits image hash.
 * @return A real number between 0 and 1. A distance of 0 means that the images are similar.
 */
double DCTpHashDistance(const DCTpHash &hash_a, const DCTpHash &hash_b);

#endif //PHASHRIS_DCTHASH_H
