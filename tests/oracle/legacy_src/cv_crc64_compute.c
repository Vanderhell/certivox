#include <certivox/cv_crc64_compute.h>

bool cv_crc64_compute(const uint8_t *data, size_t length, uint64_t *crc)
{
    if (crc == NULL || (data == NULL && length != 0u)) {
        return false;
    }
    uint64_t value = 0u;
    for (size_t i = 0u; i < length; ++i) {
        value ^= (uint64_t)data[i] << 56u;
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & (UINT64_C(1) << 63u)) != 0u
                ? (value << 1u) ^ UINT64_C(0x42F0E1EBA9EA3693)
                : value << 1u;
        }
    }
    *crc = value;
    return true;
}
