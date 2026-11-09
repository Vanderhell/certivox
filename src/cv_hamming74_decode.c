#include <certivox/cv_hamming74_decode.h>
#include <stddef.h>

cv_hamming74_status cv_hamming74_decode(uint8_t received, uint8_t *nibble)
{
    if (nibble == NULL || (received & UINT8_C(0x80)) != 0u) {
        return CV_HAMMING74_INVALID;
    }
    const uint8_t s1 = (uint8_t)(((received >> 0u) ^ (received >> 2u)
        ^ (received >> 4u) ^ (received >> 6u)) & 1u);
    const uint8_t s2 = (uint8_t)(((received >> 1u) ^ (received >> 2u)
        ^ (received >> 5u) ^ (received >> 6u)) & 1u);
    const uint8_t s4 = (uint8_t)(((received >> 3u) ^ (received >> 4u)
        ^ (received >> 5u) ^ (received >> 6u)) & 1u);
    const uint8_t syndrome = (uint8_t)(s1 | (s2 << 1u) | (s4 << 2u));
    if (syndrome != 0u) {
        received ^= (uint8_t)(1u << (syndrome - 1u));
    }
    *nibble = (uint8_t)(((received >> 2u) & 1u) | (((received >> 4u) & 1u) << 1u)
        | (((received >> 5u) & 1u) << 2u) | (((received >> 6u) & 1u) << 3u));
    return syndrome == 0u ? CV_HAMMING74_SYNDROME_ZERO : CV_HAMMING74_SYNDROME_NONZERO;
}
