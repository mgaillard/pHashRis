#include <iostream>
#include <pHash.h>
#include "RVpHash.h"

using namespace std;

/**
 * Utility function to convert a Digest into a RVpHash.
 * @param digest A Digest from the pHash library.
 * @return The corresponding RVpHash.
 */
RVpHash Digest2RVpHash(const Digest &digest) {
    RVpHash hash;

    // Copy the uint8_t array into the bitset.
    for (unsigned int i = 0; i < 40; i++)
    {
        hash <<= 8;

        RVpHash tmp_bitset(static_cast<unsigned long>(digest.coeffs[i]));

        hash |= tmp_bitset;
    }

    return hash;
}

/**
 * Utility function to convert a RvpHash into a Digest.
 * @param hash A RVpHash.
 * @param digest The corresponding Digest.
 */
void RVpHash2Digest(RVpHash hash, Digest &digest) {
    const RVpHash MASK(0xFF);
    // Copy the bitsets into the arrays
    for (unsigned int i = 0; i < 40; i++)
    {
        digest.coeffs[i] = static_cast<uint8_t>((hash & MASK).to_ulong());

        hash >>= 8;
    }
}

RVpHash RVpHashFunction(const string &file_path) {
    RVpHash hash;

    Digest digest;
    if (ph_image_digest(file_path.c_str(), 1.0, 1.0, digest, 180) == EXIT_FAILURE) {
        cerr << "Error while calculating the hash of the file " << file_path << "." << endl;
    }

    hash = Digest2RVpHash(digest);

    free(digest.coeffs);

    return hash;
}

double RVpHashDistance(const RVpHash &hash_a, const RVpHash &hash_b) {
    double dist;
    Digest digest_a, digest_b;

    digest_a.size = 40;
    digest_b.size = 40;

    // Coeffs array are allocated on the stack.
    uint8_t coeffs_a[40], coeffs_b[40];
    digest_a.coeffs = coeffs_a;
    digest_b.coeffs = coeffs_b;

    RVpHash2Digest(hash_a, digest_a);
    RVpHash2Digest(hash_b, digest_b);

    ph_crosscorr(digest_a, digest_b, dist, 0.5);

    return 1.0 - dist;
}