#include <certivox/cv_crc_update_byte_u32.h>

uint32_t cv_crc_update_byte_u32(uint32_t raw_crc, uint8_t byte, uint32_t reflected_polynomial)
{
    uint32_t value = raw_crc ^ byte;
    for (unsigned bit = 0u; bit < 8u; ++bit) {
        value = (value & UINT32_C(1)) != 0u
            ? (value >> 1u) ^ reflected_polynomial
            : value >> 1u;
    }
    return value;
}
