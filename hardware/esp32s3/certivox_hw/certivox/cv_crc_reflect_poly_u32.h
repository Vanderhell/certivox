#ifndef CV_CRC_REFLECT_POLY_U32_H
#define CV_CRC_REFLECT_POLY_U32_H
#include <stdbool.h>
#include <stdint.h>
/* Reflect the low `width` bits of a CRC polynomial (implicit x^width term
 * omitted). Width must be 1..32 and polynomial must fit that width. */
bool cv_crc_reflect_poly_u32(uint32_t polynomial, uint8_t width, uint32_t *reflected);
#endif
