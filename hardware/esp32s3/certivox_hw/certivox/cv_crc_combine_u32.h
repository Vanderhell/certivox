#ifndef CV_CRC_COMBINE_U32_H
#define CV_CRC_COMBINE_U32_H
#include <stdint.h>
/* Combine finalized CRC-32/ISO-HDLC values for A and B into CRC(A || B).
 * length_b is B's byte length; for an empty B its CRC must be zero. */
uint32_t cv_crc_combine_u32(uint32_t crc_a, uint32_t crc_b, uint32_t length_b);
#endif
