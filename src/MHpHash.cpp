#include <pHash.h>
#include "MHpHash.h"

MHpHash MHpHashFunction(const string &file_path) {
    MHpHash hash;
    uint8_t* tmp_hash;
    int tmp_hash_size;

    tmp_hash = ph_mh_imagehash(file_path.c_str(), tmp_hash_size);

    // Copy the uint8_t array into the bitset.
    for (unsigned int i = 0; i < hash.size() / 8; i++)
    {
        hash <<= 8;

        MHpHash tmp_bitset(static_cast<unsigned long>(tmp_hash[i]));

        hash |= tmp_bitset;
    }

    free(tmp_hash);

    return hash;
}

double MHpHashDistance(const MHpHash &hash_a, const MHpHash &hash_b) {
    MHpHash xor_hash(hash_a^hash_b);
    return static_cast<double>(xor_hash.count()) / hash_a.size();
}