#ifndef CERTIVOX_SECDED_CHECK_U32_H
#define CERTIVOX_SECDED_CHECK_U32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CERTIVOX_SECDED_INVALID = 0,
    CERTIVOX_SECDED_CLEAN = 1,
    CERTIVOX_SECDED_CORRECTED = 2,
    CERTIVOX_SECDED_UNCORRECTABLE = 3
} certivox_secded_status;
/* Check/correct extended Hamming(39,32). Check bits 0..5 are positional
 * parity, bit 6 is overall parity; bit 7 is invalid. Output is unchanged
 * for invalid or uncorrectable inputs. */
certivox_secded_status certivox_secded_check_u32(uint32_t received_data, uint8_t received_check_bits,
                                     uint32_t *corrected_data);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_SECDED_CHECK_U32_H */
