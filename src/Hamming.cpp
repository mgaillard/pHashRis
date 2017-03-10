#include <pHash.h>
#include "Hamming.h"

int hamming_distance(unsigned long long x, unsigned long long y) {
#ifdef POPCNT
    return popcnt_hamming_distance(x, y);
#else
    return ph_hamming_distance(x, y);
#endif
}

int popcnt_hamming_distance(unsigned long long x, unsigned long long y) {
    return __builtin_popcountll(x ^ y);
}