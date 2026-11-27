#include <certivox/hamming84_encode.h>
#include <certivox/hamming74_encode.h>
#include <stddef.h>

bool certivox_hamming84_encode(uint8_t nibble, uint8_t *codeword)
{
    if (codeword == NULL) {
        return false;
    }
    uint8_t hamming7;
    if (!certivox_hamming74_encode(nibble, &hamming7)) {
        return false;
    }
    uint8_t parity = 0u;
    for (unsigned bit = 0u; bit < 7u; ++bit) {
        parity ^= (uint8_t)((hamming7 >> bit) & 1u);
    }
    *codeword = (uint8_t)(hamming7 | (parity << 7u));
    return true;
}
