#ifndef CERTIVOX_CRC_UPDATE_BYTE_U32_H
#define CERTIVOX_CRC_UPDATE_BYTE_U32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Advance a reflected raw 32-bit CRC state by one byte. The polynomial is
 * supplied in reflected form; callers own init and final xor conventions. */
uint32_t certivox_crc_update_byte_u32(uint32_t raw_crc, uint8_t byte, uint32_t reflected_polynomial);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_CRC_UPDATE_BYTE_U32_H */
