#ifndef CV_HAMMING84_DECODE_H
#define CV_HAMMING84_DECODE_H
#include <stdint.h>
typedef enum {
    CV_HAMMING84_INVALID = 0,
    CV_HAMMING84_CLEAN = 1,
    CV_HAMMING84_CORRECTED = 2,
    CV_HAMMING84_UNCORRECTABLE = 3
} cv_hamming84_status;
/* Decode extended even-parity Hamming(8,4): correct single errors, detect
 * double errors. Output is unchanged on invalid or uncorrectable input. */
cv_hamming84_status cv_hamming84_decode(uint8_t received, uint8_t *nibble);
#endif
