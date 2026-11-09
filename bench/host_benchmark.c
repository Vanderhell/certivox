#include <certivox/cv_crc32_compute.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
    static const uint8_t payload[32] = { 0u };
    volatile uint32_t sink = 0u;
    const clock_t start = clock();
    for (unsigned i = 0u; i < 100000u; ++i) {
        uint32_t crc = 0u;
        (void)cv_crc32_compute(payload, sizeof(payload), &crc);
        sink ^= crc;
    }
    const clock_t elapsed = clock() - start;
    printf("host_clock_ticks=%lu sink=%u\n", (unsigned long)elapsed, (unsigned)sink);
    return sink == UINT32_MAX;
}
