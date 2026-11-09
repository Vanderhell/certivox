#ifndef CV_CRC_UPDATE_BYTE_U32_H
#define CV_CRC_UPDATE_BYTE_U32_H
#include <stdint.h>
/* Advance a reflected raw 32-bit CRC state by one byte. The polynomial is
 * supplied in reflected form; callers own init and final xor conventions. */
uint32_t cv_crc_update_byte_u32(uint32_t raw_crc, uint8_t byte, uint32_t reflected_polynomial);
#endif
