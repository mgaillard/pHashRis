#ifndef PHASHRIS_MHPHASH_H
#define PHASHRIS_MHPHASH_H

#include <bitset>
#include <string>

using namespace std;

/**
 * A 576 bits hash.
 */
typedef bitset<576> MHpHash;

/**
 * Compute the MH based image hash of an image.
 * @param file_path A path to an image.
 * @return A 576 bits image hash.
 */
MHpHash MHpHashFunction(const string &file_path);

/**
 * Compute the distance between two MH based image hashes.
 * The Hamming distance is used.
 * @param hash_a A 576 bits image hash.
 * @param hash_b A 576 bits image hash.
 * @return A real number between 0 and 1. A distance of 0 means that the images are similar.
 */
double MHpHashDistance(const MHpHash &hash_a, const MHpHash &hash_b);

#endif //PHASHRIS_MHPHASH_H
