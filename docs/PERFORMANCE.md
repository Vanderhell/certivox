# Performance and memory

## Historical pre-productization measurements

Previously measured on an ESP32-S3 (revision 0.2, 240 MHz, 40 MHz XTAL), before the current namespace and source-layout migration, using ESP-IDF 5.5.1, Xtensa GCC 14.2.0, `-O2`, and production `src/*.c` compiled as C99. The validation image ran all 14 public API checks successfully. Each timing is the median and min/max across 21 samples, with a paired no-op batch subtracted. The measured empty-loop/no-op baseline was 15 cycles per call. `certivox_crc_combine_u32` used length 32,768; CRC buffer APIs used 32 bytes; the remaining inputs are listed in the metadata below. Numbers are cycles/call, not wall-clock estimates. The CRC timing varies with payload contents and size.

| API | Representative input | Min | Median | Max |
|---|---|---:|---:|---:|
| `certivox_crc8_compute` | 32-byte deterministic payload | 2,128.03 | 2,128.47 | 2,129.94 |
| `certivox_crc16_compute` | 32-byte deterministic payload | 1,873.04 | 1,874.48 | 1,875.48 |
| `certivox_crc24_compute` | 32-byte deterministic payload | 2,191.65 | 2,193.48 | 2,194.49 |
| `certivox_crc32_compute` | 32-byte deterministic payload | 1,610.03 | 1,611.48 | 1,612.50 |
| `certivox_crc64_compute` | 32-byte deterministic payload | 2,673.45 | 2,673.66 | 2,675.94 |
| `certivox_crc_update_byte_u32` | state `A5A55A5A`, byte `3C`, poly `EDB88320` | 71.86 | 72.16 | 73.14 |
| `certivox_crc_combine_u32` | CRCs `12345678`, `9ABCDEF0`, length 32768 | 133,292.92 | 133,297.84 | 133,353.28 |
| `certivox_crc_reflect_poly_u32` | polynomial `04C11DB7`, width 32 | 306.01 | 307.00 | 308.45 |
| `certivox_hamming74_encode` | nibble `0B` | 65.01 | 65.37 | 67.45 |
| `certivox_hamming74_decode` | codeword `6D` | 77.01 | 78.01 | 78.45 |
| `certivox_hamming84_encode` | nibble `0B` | 123.01 | 123.99 | 124.49 |
| `certivox_hamming84_decode` | codeword `B6` | 124.01 | 125.01 | 125.45 |
| `certivox_secded_parity_u32` | data `A55A1234` | 398.94 | 400.01 | 401.45 |
| `certivox_secded_check_u32` | data `A55A1234`, independently computed checks | 1,645.02 | 1,646.48 | 1,647.52 |

The benchmark dispatches through volatile function pointers and folds every result into a volatile sink to prevent elimination. These are historical board observations from before the current namespace and source-layout migration; they are not measurements of the current revision or universal timing guarantees. ESP-IDF boot/runtime and the test harness are not included in the isolated library size below.

The historical linker map attributes 1,548 bytes of linked library code to its 14 API translation units in this all-APIs-referenced image:

| API translation unit | Linked flash bytes | Static RAM bytes |
|---|---:|---:|
| `certivox_crc8_compute.c` | 78 | 0 |
| `certivox_crc16_compute.c` | 88 | 0 |
| `certivox_crc24_compute.c` | 90 | 0 |
| `certivox_crc32_compute.c` | 72 | 0 |
| `certivox_crc64_compute.c` | 106 | 0 |
| `certivox_crc_update_byte_u32.c` | 33 | 0 |
| `certivox_crc_combine_u32.c` | 232 | 0 |
| `certivox_crc_reflect_poly_u32.c` | 84 | 0 |
| `certivox_hamming74_encode.c` | 106 | 0 |
| `certivox_hamming74_decode.c` | 183 | 0 |
| `certivox_hamming84_encode.c` | 61 | 0 |
| `certivox_hamming84_decode.c` | 180 | 0 |
| `certivox_secded_parity_u32.c` | 85 | 0 |
| `certivox_secded_check_u32.c` | 150 | 0 |

## Earlier ESP32-S3 measurements

The results below are retained from existing device reports dated 2026-11-09; they predate the current productization and are not current-revision measurements. The reports identify ESP32-S3 at 240 MHz with Arduino-ESP32 3.3.10 / Xtensa GCC 14.2.0. Each timing is a reported mean and min/max range on its documented representative input, not a current verification claim.

| API | Historical timing at 240 MHz |
|---|---:|
| `certivox_crc8_compute`, 32 bytes | mean 2,549.01 cycles/call; range 2,548.99Ă˘â‚¬â€ś2,549.10 |
| `certivox_crc16_compute`, 32 bytes | mean 2,004.43; range 2,004.34Ă˘â‚¬â€ś2,004.50 |
| `certivox_crc24_compute`, 32 bytes | mean 2,455.59; range 2,455.56Ă˘â‚¬â€ś2,455.67 |
| `certivox_crc32_compute`, 32 bytes | mean 1,748.34; range 1,748.25Ă˘â‚¬â€ś1,748.40 |
| `certivox_crc64_compute`, 32 bytes | mean 2,587.19; range 2,587.10Ă˘â‚¬â€ś2,587.24 |
| `certivox_crc_update_byte_u32`, one byte | mean 73.34; range 73.34Ă˘â‚¬â€ś73.44 |
| `certivox_crc_combine_u32`, suffix 32,768Ă˘â‚¬â€ś32,867 bytes | mean 209,726.50; range 209,720.04Ă˘â‚¬â€ś209,736.88 |
| `certivox_crc_reflect_poly_u32`, width 1 / 32 | mean 58.29 / 306.56 |
| `certivox_hamming74_encode` | mean 64.32; range 64.31Ă˘â‚¬â€ś64.33 |
| `certivox_hamming74_decode` | mean 79.90; range 79.89Ă˘â‚¬â€ś79.90 |
| `certivox_hamming84_encode` | mean 129.64; range 129.64Ă˘â‚¬â€ś129.65 |
| `certivox_hamming84_decode` | mean 121.15; range 121.14Ă˘â‚¬â€ś121.15 |
| `certivox_secded_parity_u32` | mean 370.77; range 370.76Ă˘â‚¬â€ś370.78 |
| `certivox_secded_check_u32` | mean 2,926.48; range 2,926.47Ă˘â‚¬â€ś2,926.50 |

These reported timings use the historical harness and may include harness characteristics; the original detailed reports remain in the internal evidence archive. They are not a best/median/worst profile for the current audit.

## Memory records

The old all-in-one Arduino test firmware measured 311,592 bytes program storage and 22,248 bytes global RAM, including Arduino/runtime and harness. These totals are **not** Certivox library size. A historical A/B link-size comparison for a minimal build containing only `certivox_crc32_compute` reported a 104-byte flash delta and 0-byte global-RAM delta. This is not an all-module footprint measurement.

| Historical-audit metric | Result |
|---|---|
| Isolated complete-library linked flash contribution (`libcertivox.a`, all 14 APIs referenced) | 1,548 bytes (`.flash.text`, per-file map sum) |
| Isolated library static RAM contribution | 0 bytes in linked `.dram0.data` / `.dram0.bss` across all API translation units |
| Whole validation app binary | 194,320 bytes (includes ESP-IDF runtime and validation harness; not library size) |
| Main-task stack watermark | 1,652 bytes observed use from a 3,584-byte task stack; 1,932 bytes minimum free, word size 1 byte |

Footprint values are ESP-IDF 5.5.1 linker map/component accounting for this configuration; individual translation-unit values depend on this link configuration. The stack watermark is a single observed high-water mark for this test run, not a maximum stack bound. Historical Arduino measurements above remain historical and separate.
