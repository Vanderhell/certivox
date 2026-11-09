# ESP32-S3 result: `cv_secded_check_u32`

## Purpose and category

Checks and corrects extended Hamming(39,32) data plus seven check bits. Category: software SECDED for 32-bit SRAM, storage records, or retained state without hardware ECC.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=134|build=20261029a`.
- Sketch footprint: 310,868 bytes flash and 22,192 bytes static RAM including runtime, independent oracle, and harness; not primitive-only. Production uses no table or heap and leaves data/output ownership with the caller.

## Tests and results

- PASS: 32,768 low-byte-data/valid-check combinations matched independent mask-based parity/syndrome classification.
- PASS: all 128 reserved-bit check-byte values were rejected without changing the output sentinel.
- PASS: all 39 single-bit errors on `0x12345678` corrected; all 741 pairs of distinct bit errors reported UNCORRECTABLE with output unchanged.
- PASS: 100,000 seeded full-width data/check-byte cases matched the oracle; seed `0x524D0134`, checksum `3678338170`.
- 10 batches of 100,000 valid clean-word calls: mean 2,926.48 cycles/call, range 2,926.47..2,926.50 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_secded_check_u32|PASS|exhaustive=32768|singles=39|doubles=741|reserved_invalid=128|stress=100000|checksum=3678338170|batch=100000|mean_cycles_x100=292648|min_cycles_x100=292647|max_cycles_x100=292650|cpu_mhz=240
```

## Use and limits

Use when software must correct a single bit and flag a double-bit error in a fixed 32-bit codeword. At about 12.2 microseconds per word on this 240 MHz ESP32-S3, it is suited to occasional memory/storage checks; high-rate paths should use hardware ECC or benchmark an optimized layout. Three or more errors can be miscorrected or appear clean; SECDED is not cryptographic integrity.
