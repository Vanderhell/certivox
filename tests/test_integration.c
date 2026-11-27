#include <certivox/crc32_compute.h>
#include <certivox/hamming84_decode.h>
#include <certivox/hamming84_encode.h>
#include <certivox/secded_check_u32.h>
#include <certivox/secded_parity_u32.h>
#include <stdint.h>
#include <stdio.h>

#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "failed line %d\n", __LINE__); return 1; } } while (0)

int main(void)
{
    static const uint8_t message[] = "certivox";
    uint32_t crc = 0u;
    CHECK(certivox_crc32_compute(message, sizeof(message) - 1u, &crc));
    CHECK(crc == UINT32_C(0x0299D3BB));

    uint8_t encoded = 0u;
    uint8_t decoded = 0u;
    CHECK(certivox_hamming84_encode(UINT8_C(0x0B), &encoded));
    CHECK(certivox_hamming84_decode((uint8_t)(encoded ^ UINT8_C(0x04)), &decoded) == CERTIVOX_HAMMING84_CORRECTED);
    CHECK(decoded == UINT8_C(0x0B));

    const uint32_t word = UINT32_C(0xA55A1234);
    uint8_t check_bits = 0u;
    uint32_t corrected = 0u;
    CHECK(certivox_secded_parity_u32(word, &check_bits));
    CHECK(certivox_secded_check_u32(word, check_bits, &corrected) == CERTIVOX_SECDED_CLEAN);
    CHECK(corrected == word);
    return 0;
}
