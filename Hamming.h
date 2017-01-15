#ifndef HAMMING_H
#define HAMMING_H

/**
 * Compute the hamming distance between two unsigned 64 bits integers.
 * @param x An unsigned 64 bits integer
 * @param y An unsigned 64 bits integer
 * @return The hamming distance between x and y
 */
int hamming_distance(unsigned long long x, unsigned long long y);

/**
 * Compute the hamming distance between two unsigned 64 bits integers.
 * Make use of the special popcount gcc intrinsics.
 * @param x An unsigned 64 bits integer
 * @param y An unsigned 64 bits integer
 * @return The hamming distance between x and y
 */
int popcnt_hamming_distance(unsigned long long x, unsigned long long y);

#endif //HAMMING_H
