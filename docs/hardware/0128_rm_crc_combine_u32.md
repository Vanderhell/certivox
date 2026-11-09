# ESP32-S3 result: `cv_crc_combine_u32`

## Purpose and category

Combines finalized CRC-32/ISO-HDLC values for two segments using only their CRC values and the suffix byte length. This is a specialized segmented-data, storage, and communications utility for cases where segment bytes need not be reread.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Firmware boot marker: `RMBOOT|candidate=128|build=20261029b`.
- Sketch: 310,656 bytes flash and 25,264 bytes static RAM. These totals include the runtime, test oracle, lookup table, data buffers, and harness; they are not primitive-only. Primitive stack workspace: 256 bytes for two 32-word GF(2) matrices.

## Test and performance results

- PASS: 65,536 exhaustive one-byte segment pairs.
- PASS: 100,000 deterministic randomized segment pairs, both lengths independently in `[0, 512]`, checked against an independent table-driven CRC of the concatenated bytes.
- PASS: empty-prefix and empty-suffix behavior and the standard `123456789` result `0xCBF43926`.
- Seed: `0x524D0128`; result checksum: `1005152771`.
- 25 timing batches of 100 combinations, suffix sizes 32,768..32,867 bytes: mean `20,972,650` cycles per 100 calls, or `209,726.50` cycles/call (`~874 us` at 240 MHz); min/max `209,720.04` / `209,736.88` cycles/call.

## Raw serial record

```text
RMDEVICE|cv_crc_combine_u32|PASS|stress=100000|exhaustive=65536|checksum=1005152771|batch=100|mean_cycles_x100=20972650|min_cycles_x100=20972004|max_cycles_x100=20973688|cpu_mhz=240
```

## Use and limits

Use when independently processed large chunks must be combined after their bytes are unavailable, or when rereading a suffix is more expensive than the measured matrix operation. The contract only supports finalized CRC-32/ISO-HDLC values and a 32-bit suffix length. The fixed CRC profile and 256-byte stack workspace make this unsuitable as a general CRC API; for short or resident data, simply updating/recomputing the CRC is usually preferable.
