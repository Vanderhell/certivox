#ifndef CV_HAMMING74_DECODE_H
#define CV_HAMMING74_DECODE_H
#include <stdint.h>
typedef enum {
    CV_HAMMING74_INVALID = 0,
    CV_HAMMING74_SYNDROME_ZERO = 1,
    CV_HAMMING74_SYNDROME_NONZERO = 2
} cv_hamming74_status;
/* Decode even-parity Hamming(7,4). A nonzero syndrome flips its indicated
 * bit and is reported as such; only a single-bit error is guaranteed corrected. */
cv_hamming74_status cv_hamming74_decode(uint8_t received, uint8_t *nibble);
#endif
