# ESP32-S3 result: `cv_hamming84_decode`

## Purpose and category

Decodes extended even-parity Hamming(8,4), correcting any single bit and flagging any double-bit error as uncorrectable. Category: compact SECDED decoding for short communications or stored symbols.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=132|build=20261029a`.
- Sketch footprint: 311,040 bytes flash and 22,192 bytes static RAM including runtime, independent oracle, and harness; not primitive-only. Decoder itself uses scalar local state, no heap or tables.

## Tests and results

- PASS: all 256 received bytes matched an independent nearest-codeword oracle: 16 CLEAN, 128 CORRECTED, 112 UNCORRECTABLE. On UNCORRECTABLE, the output sentinel remained unchanged; null output returned INVALID.
- PASS: all 128 single-bit corruptions corrected to the original nibble, including the overall parity bit.
- PASS: all 448 two-bit corruptions returned UNCORRECTABLE and preserved output.
- PASS: 1,000,000 deterministic received-byte cases matched the oracle; checksum `2481628267`.
- 10 batches of 100,000 decodes: mean 121.15 cycles/call; range 121.14..121.15 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_hamming84_decode|PASS|exhaustive=256|clean=16|corrected=128|uncorrectable=112|single=128|double=448|longrun=1000000|checksum=2481628267|batch=100000|mean_cycles_x100=12115|min_cycles_x100=12114|max_cycles_x100=12115|cpu_mhz=240
```

## Use and limits

Use with the matching Hamming(8,4) encoder where 50% payload efficiency is acceptable and single-error correction / double-error detection is useful. Three or more errors are outside the SECDED guarantee and can be miscorrected. For security or burst corruption, use an authenticated integrity mechanism or a code/interleaving scheme designed for that channel.
