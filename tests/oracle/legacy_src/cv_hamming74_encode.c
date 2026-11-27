#include <certivox/cv_hamming74_encode.h>
#include <stddef.h>

bool cv_hamming74_encode(uint8_t nibble, uint8_t *codeword)
{
    if (codeword == NULL || nibble > UINT8_C(0x0F)) {
        return false;
    }
    const uint8_t d0 = nibble & 1u;
    const uint8_t d1 = (nibble >> 1u) & 1u;
    const uint8_t d2 = (nibble >> 2u) & 1u;
    const uint8_t d3 = (nibble >> 3u) & 1u;
    const uint8_t p1 = d0 ^ d1 ^ d3;
    const uint8_t p2 = d0 ^ d2 ^ d3;
    const uint8_t p4 = d1 ^ d2 ^ d3;
    *codeword = (uint8_t)(p1 | (p2 << 1u) | (d0 << 2u) | (p4 << 3u)
        | (d1 << 4u) | (d2 << 5u) | (d3 << 6u));
    return true;
}
