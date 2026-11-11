# Performance and memory

## Current audit status

**HW NOT VERIFIED.** COM37 is reserved, and COM19/COM36 may hold firmware and data belonging to other projects. They were enumerated read-only and not opened or modified. No current hardware cycle, isolated footprint or stack measurement was performed for this audit.

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
| Per-API isolated flash contribution | NOT MEASURED |
| Complete-library isolated flash contribution | NOT MEASURED |
| Per-module/static RAM contribution | NOT MEASURED |
| Stack use | NOT MEASURED |

No current stack bounds or maximum stack claim is available. Host `bench/host_benchmark.c` is a smoke benchmark and does not produce MCU cycle counts.
