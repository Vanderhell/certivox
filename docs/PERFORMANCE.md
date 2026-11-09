# Performance

## Extracted ESP32-S3 validation

The following result was measured on 2026-11-09 using an ESP32-S3 QFN56
revision 0.2 board at 240 MHz, USB-Serial/JTAG on COM37, Arduino ESP32 core
3.3.10, and the `esp32:esp32:esp32s3:CDCOnBoot=cdc,UploadSpeed=921600`
configuration. The firmware ran 10,000 seeded randomized CRC32 comparisons,
exhaustive Hamming(7,4) single-bit cases, Hamming(8,4) correction/rejection
cases, SECDED clean-state checks, and CRC known vectors.

| Measurement | Result | Scope |
|---|---:|---|
| Hardware test result | PASS | Complete extracted test sketch |
| CRC32 cycles for 32-byte input, minimum | 1,765 | 10 batches of 10,000 calls |
| CRC32 cycles for 32-byte input, maximum | 1,765 | 10 batches of 10,000 calls |
| Test firmware program storage | 311,592 bytes | Arduino/runtime + complete harness |
| Test firmware global RAM | 22,248 bytes | Arduino/runtime + complete harness |

## Isolated product delta

An A/B build used the same Arduino configuration, serial output and loop
structure. The baseline contained no Certivox code; the minimal build linked
only `cv_crc32_compute`. This is a link-time delta, not a whole-firmware
estimate for all fourteen public functions.

| Build | Program storage | Global RAM |
|---|---:|---:|
| Baseline without Certivox | 309,364 bytes | 22,184 bytes |
| Minimal build with CRC32 | 309,468 bytes | 22,184 bytes |
| Measured delta | 104 bytes | 0 bytes |

Stack, heap and per-function deltas were not measured.

## Inherited source records

The source ESP32-S3 records report 240 MHz measurements for the original
implementations. They are retained for traceability only. Reported firmware
sizes include Arduino/runtime, harness, test buffers and oracle data; they are
not Certivox-only `.text`, `.rodata`, `.data` or `.bss` measurements.

| Group | Hardware status | Measurement status |
|---|---|---|
| CRC profiles and helpers | ESP32-S3 PASS in source records | Retained for provenance; harness-inclusive |
| Hamming(7,4)/(8,4) | ESP32-S3 PASS in source records | Retained for provenance; harness-inclusive |
| SECDED(39,32) | ESP32-S3 PASS in source records | Retained for provenance; harness-inclusive |

## Host benchmark

`bench/host_benchmark.c` is a smoke benchmark only. Host clock ticks are not
MCU cycle measurements and must not be compared with the ESP32-S3 records.

The host benchmark is not an MCU cycle measurement. The isolated delta above
must not be generalized to the complete product without another per-module
link-time measurement.
