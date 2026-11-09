#ifndef CV_HAMMING84_ENCODE_H
#define CV_HAMMING84_ENCODE_H
#include <stdbool.h>
#include <stdint.h>
/* Encode nibble with Hamming(8,4): positions 1..7 match Hamming(7,4),
 * bit 7 is overall even parity. */
bool cv_hamming84_encode(uint8_t nibble, uint8_t *codeword);
#endif
