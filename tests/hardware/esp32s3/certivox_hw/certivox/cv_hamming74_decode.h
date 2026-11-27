#ifndef CERTIVOX_HAMMING74_DECODE_H
#define CERTIVOX_HAMMING74_DECODE_H
#include <stdint.h>
typedef enum {
    CERTIVOX_HAMMING74_INVALID = 0,
    CERTIVOX_HAMMING74_SYNDROME_ZERO = 1,
    CERTIVOX_HAMMING74_SYNDROME_NONZERO = 2
} certivox_hamming74_status;
/* Decode even-parity Hamming(7,4). A nonzero syndrome flips its indicated
 * bit and is reported as such; only a single-bit error is guaranteed corrected. */
certivox_hamming74_status certivox_hamming74_decode(uint8_t received, uint8_t *nibble);
#endif
