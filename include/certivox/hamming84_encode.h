#ifndef CERTIVOX_HAMMING84_ENCODE_H
#define CERTIVOX_HAMMING84_ENCODE_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Encode nibble with Hamming(8,4): positions 1..7 match Hamming(7,4),
 * bit 7 is overall even parity. */
bool certivox_hamming84_encode(uint8_t nibble, uint8_t *codeword);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_HAMMING84_ENCODE_H */
