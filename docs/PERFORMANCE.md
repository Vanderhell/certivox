# Performance and memory

## Current audit measurements

Newly measured on the user-authorized ESP32-S3 at COM37 (ESP32-S3 rev. 0.2, 240 MHz, 40 MHz XTAL), using ESP-IDF 5.5.1, Xtensa GCC 14.2.0, `-O2`, and production `src/*.c` compiled as C99. The validation image ran all 14 public API checks successfully. Each timing is the median and min/max across 21 samples, with a paired no-op batch subtracted. The measured empty-loop/no-op baseline was 15 cycles per call. `cv_crc_combine_u32` used length 32,768; CRC buffer APIs used 32 bytes; the remaining inputs are listed in the metadata below. Numbers are cycles/call, not wall-clock estimates. The CRC timing varies with payload contents and size.

| API | Representative input | Min | Median | Max |
|---|---|---:|---:|---:|
| `cv_crc8_compute` | 32-byte deterministic payload | 2,128.03 | 2,128.47 | 2,129.94 |
| `cv_crc16_compute` | 32-byte deterministic payload | 1,873.04 | 1,874.48 | 1,875.48 |
| `cv_crc24_compute` | 32-byte deterministic payload | 2,191.65 | 2,193.48 | 2,194.49 |
| `cv_crc32_compute` | 32-byte deterministic payload | 1,610.03 | 1,611.48 | 1,612.50 |
| `cv_crc64_compute` | 32-byte deterministic payload | 2,673.45 | 2,673.66 | 2,675.94 |
| `cv_crc_update_byte_u32` | state `A5A55A5A`, byte `3C`, poly `EDB88320` | 71.86 | 72.16 | 73.14 |
| `cv_crc_combine_u32` | CRCs `12345678`, `9ABCDEF0`, length 32768 | 133,292.92 | 133,297.84 | 133,353.28 |
| `cv_crc_reflect_poly_u32` | polynomial `04C11DB7`, width 32 | 306.01 | 307.00 | 308.45 |
| `cv_hamming74_encode` | nibble `0B` | 65.01 | 65.37 | 67.45 |
| `cv_hamming74_decode` | codeword `6D` | 77.01 | 78.01 | 78.45 |
| `cv_hamming84_encode` | nibble `0B` | 123.01 | 123.99 | 124.49 |
| `cv_hamming84_decode` | codeword `B6` | 124.01 | 125.01 | 125.45 |
| `cv_secded_parity_u32` | data `A55A1234` | 398.94 | 400.01 | 401.45 |
| `cv_secded_check_u32` | data `A55A1234`, independently computed checks | 1,645.02 | 1,646.48 | 1,647.52 |

The benchmark dispatches through volatile function pointers and folds every result into a volatile sink to prevent elimination. These are current board observations under this harness and build, not universal timing guarantees. ESP-IDF boot/runtime and the test harness are not included in the isolated library size below.

## Historical ESP32-S3 evidence

The results below are retained from existing device reports dated 2026-11-09; they are not newly executed measurements. Individual module reports identify ESP32-S3 at 240 MHz on COM36 with Arduino-ESP32 3.3.10 / Xtensa GCC 14.2.0. The extracted all-in-one sketch report identifies COM37. Each timing is a reported mean and min/max range on its documented representative input, not a current verification claim.

| API | Historical timing at 240 MHz |
|---|---:|
| `cv_crc8_compute`, 32 bytes | mean 2,549.01 cycles/call; range 2,548.99â€“2,549.10 |
| `cv_crc16_compute`, 32 bytes | mean 2,004.43; range 2,004.34â€“2,004.50 |
| `cv_crc24_compute`, 32 bytes | mean 2,455.59; range 2,455.56â€“2,455.67 |
| `cv_crc32_compute`, 32 bytes | mean 1,748.34; range 1,748.25â€“1,748.40 |
| `cv_crc64_compute`, 32 bytes | mean 2,587.19; range 2,587.10â€“2,587.24 |
| `cv_crc_update_byte_u32`, one byte | mean 73.34; range 73.34â€“73.44 |
| `cv_crc_combine_u32`, suffix 32,768â€“32,867 bytes | mean 209,726.50; range 209,720.04â€“209,736.88 |
| `cv_crc_reflect_poly_u32`, width 1 / 32 | mean 58.29 / 306.56 |
| `cv_hamming74_encode` | mean 64.32; range 64.31â€“64.33 |
| `cv_hamming74_decode` | mean 79.90; range 79.89â€“79.90 |
| `cv_hamming84_encode` | mean 129.64; range 129.64â€“129.65 |
| `cv_hamming84_decode` | mean 121.15; range 121.14â€“121.15 |
| `cv_secded_parity_u32` | mean 370.77; range 370.76â€“370.78 |
| `cv_secded_check_u32` | mean 2,926.48; range 2,926.47â€“2,926.50 |

These reported timings use the historical harness and may include harness characteristics; consult each `docs/hardware/*.md` record for its exact batch method and test inputs. They are not a best/median/worst profile for the current audit.

## Memory records

The old all-in-one Arduino test firmware measured 311,592 bytes program storage and 22,248 bytes global RAM, including Arduino/runtime and harness. These totals are **not** Certivox library size. A historical A/B link-size comparison for a minimal build containing only `cv_crc32_compute` reported a 104-byte flash delta and 0-byte global-RAM delta. This is not an all-module footprint measurement.

| Current-audit metric | Result |
|---|---|
| Isolated complete-library linked flash contribution (`libcertivox.a`, all 14 APIs referenced) | 1,548 bytes (`.flash.text`) |
| Isolated library static RAM contribution | 0 bytes in linked `.dram0.data` / `.dram0.bss` |
| Whole validation app binary | 194,320 bytes (includes ESP-IDF runtime and validation harness; not library size) |
| Main-task stack watermark | 1,652 bytes observed use from a 3,584-byte task stack; 1,932 bytes minimum free, word size 1 byte |

Footprint values are ESP-IDF 5.5.1 linker map/component accounting for this configuration. Per-API isolated flash values were not measured. The stack watermark is a single observed high-water mark for this test run, not a maximum stack bound. Historical Arduino measurements above remain historical and separate.
