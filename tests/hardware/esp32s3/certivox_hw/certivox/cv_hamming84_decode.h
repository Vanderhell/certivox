#ifndef CERTIVOX_HAMMING84_DECODE_H
#define CERTIVOX_HAMMING84_DECODE_H
#include <stdint.h>
typedef enum {
    CERTIVOX_HAMMING84_INVALID = 0,
    CERTIVOX_HAMMING84_CLEAN = 1,
    CERTIVOX_HAMMING84_CORRECTED = 2,
    CERTIVOX_HAMMING84_UNCORRECTABLE = 3
} certivox_hamming84_status;
/* Decode extended even-parity Hamming(8,4): correct single errors, detect
 * double errors. Output is unchanged on invalid or uncorrectable input. */
certivox_hamming84_status certivox_hamming84_decode(uint8_t received, uint8_t *nibble);
#endif
