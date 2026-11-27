#include <certivox/cv_crc24_compute.h>

bool cv_crc24_compute(const uint8_t *data, size_t length, uint32_t *crc)
{
    if (crc == NULL || (data == NULL && length != 0u)) {
        return false;
    }
    uint32_t value = UINT32_C(0xB704CE);
    for (size_t i = 0u; i < length; ++i) {
        value ^= (uint32_t)data[i] << 16u;
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & UINT32_C(0x800000)) != 0u
                ? ((value << 1u) ^ UINT32_C(0x864CFB)) & UINT32_C(0xFFFFFF)
                : (value << 1u) & UINT32_C(0xFFFFFF);
        }
    }
    *crc = value;
    return true;
}
