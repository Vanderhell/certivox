#ifndef CV_HAMMING74_ENCODE_H
#define CV_HAMMING74_ENCODE_H
#include <stdbool.h>
#include <stdint.h>
/* Encode nibble d3..d0 into even-parity Hamming(7,4), positions numbered
 * 1..7 in code bits 0..6; parity occupies positions 1,2,4. */
bool cv_hamming74_encode(uint8_t nibble, uint8_t *codeword);
#endif
