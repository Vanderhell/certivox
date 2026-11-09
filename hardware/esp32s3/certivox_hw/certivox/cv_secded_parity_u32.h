#ifndef CV_SECDED_PARITY_U32_H
#define CV_SECDED_PARITY_U32_H
#include <stdbool.h>
#include <stdint.h>
/* Return seven check bits for extended even-parity Hamming(39,32): bits 0..5
 * are Hamming parity for positions 1,2,4,8,16,32; bit 6 is overall parity. */
bool cv_secded_parity_u32(uint32_t data, uint8_t *check_bits);
#endif
