#include <certivox/cv_hamming84_decode.h>
#include <stddef.h>

cv_hamming84_status cv_hamming84_decode(uint8_t received, uint8_t *nibble)
{
    if (nibble == NULL) {
        return CV_HAMMING84_INVALID;
    }
    const uint8_t s1 = (uint8_t)(((received >> 0u) ^ (received >> 2u)
        ^ (received >> 4u) ^ (received >> 6u)) & 1u);
    const uint8_t s2 = (uint8_t)(((received >> 1u) ^ (received >> 2u)
        ^ (received >> 5u) ^ (received >> 6u)) & 1u);
    const uint8_t s4 = (uint8_t)(((received >> 3u) ^ (received >> 4u)
        ^ (received >> 5u) ^ (received >> 6u)) & 1u);
    const uint8_t syndrome = (uint8_t)(s1 | (s2 << 1u) | (s4 << 2u));
    uint8_t overall = 0u;
    for (unsigned bit = 0u; bit < 8u; ++bit) {
        overall ^= (uint8_t)((received >> bit) & 1u);
    }
    if (syndrome != 0u && overall == 0u) {
        return CV_HAMMING84_UNCORRECTABLE;
    }
    if (syndrome != 0u) {
        received ^= (uint8_t)(1u << (syndrome - 1u));
    } else if (overall != 0u) {
        received ^= UINT8_C(0x80);
    }
    *nibble = (uint8_t)(((received >> 2u) & 1u) | (((received >> 4u) & 1u) << 1u)
        | (((received >> 5u) & 1u) << 2u) | (((received >> 6u) & 1u) << 3u));
    return overall == 0u ? CV_HAMMING84_CLEAN : CV_HAMMING84_CORRECTED;
}
