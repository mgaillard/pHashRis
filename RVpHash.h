#ifndef PHASHRIS_RVPHASH_H
#define PHASHRIS_RVPHASH_H

#include <bitset>

using namespace std;

/**
 * A 320 bits hash.
 */
typedef bitset<320> RVpHash;

/**
 * Compute the RV based image hash of an image.
 * @param file_path A path to an image.
 * @return A 320 bits image hash.
 */
RVpHash RVpHashFunction(const string &file_path);

/**
 * Compute the distance between two RV based image hashes.
 * The cross correlation distance is used.
 * @param hash_a A 320 bits image hash.
 * @param hash_b A 320 bits image hash.
 * @return A real number between 0 and 1. A distance of 0 means that the images are similar.
 */
double RVpHashDistance(const RVpHash &hash_a, const RVpHash &hash_b);

#endif //PHASHRIS_RVPHASH_H
