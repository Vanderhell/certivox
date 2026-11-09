#include <certivox/cv_crc32_compute.h>
#include <certivox/cv_hamming84_decode.h>
#include <certivox/cv_hamming84_encode.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    static const uint8_t payload[] = "certivox";
    uint32_t crc = 0u;
    uint8_t codeword = 0u;
    uint8_t nibble = 0u;
    if (!cv_crc32_compute(payload, sizeof(payload) - 1u, &crc)
        || !cv_hamming84_encode(UINT8_C(0x0Au), &codeword)
        || cv_hamming84_decode(codeword, &nibble) != CV_HAMMING84_CLEAN) {
        return 1;
    }
    printf("crc32=0x%08X nibble=0x%X\n", (unsigned)crc, (unsigned)nibble);
    return 0;
}
