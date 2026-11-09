#ifndef CV_SECDED_CHECK_U32_H
#define CV_SECDED_CHECK_U32_H
#include <stdint.h>
typedef enum {
    CV_SECDED_INVALID = 0,
    CV_SECDED_CLEAN = 1,
    CV_SECDED_CORRECTED = 2,
    CV_SECDED_UNCORRECTABLE = 3
} cv_secded_status;
/* Check/correct extended Hamming(39,32). Check bits 0..5 are positional
 * parity, bit 6 is overall parity; bit 7 is invalid. Output is unchanged
 * for invalid or uncorrectable inputs. */
cv_secded_status cv_secded_check_u32(uint32_t received_data, uint8_t received_check_bits,
                                     uint32_t *corrected_data);
#endif
