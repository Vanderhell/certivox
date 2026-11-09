# ESP32-S3 result: `cv_hamming74_encode`

## Purpose and category

Encodes a 4-bit symbol as an even-parity Hamming(7,4) codeword. Category: compact communications / error-correction coding for short fields where endpoints agree on this exact bit order.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=129|build=20261029a`.
- Sketch footprint: 310,636 bytes flash and 22,192 bytes static RAM including runtime, oracle, and harness; not primitive-only. The production encoder itself uses scalar locals only, with no table, heap, or persistent state.

## Tests and results

- PASS: exhaustive byte input domain 0..255. Inputs 0..15 matched the independent position-based parity oracle; all other 240 values were rejected and preserved the output byte.
- PASS: null output rejected for both a valid and invalid symbol.
- PASS: all 240 ordered distinct codeword pairs have Hamming distance at least 3.
- PASS: 112 one-bit corruptions have distance at least 2 from every other valid codeword.
- PASS: 1,000,000 deterministic encodes matched the oracle; checksum `1768560128`.
- 10 timing batches of 100,000 calls: mean 64.32 cycles/call, range 64.31..64.33 cycles/call (240 MHz).

## Raw serial result

```text
RMDEVICE|cv_hamming74_encode|PASS|exhaustive=256|pairs=240|single_bits=112|longrun=1000000|checksum=1768560128|batch=100000|mean_cycles_x100=6432|min_cycles_x100=6431|max_cycles_x100=6433|cpu_mhz=240
```

## Use and limits

Useful for compact control symbols, short radio/serial fields, and constrained links that benefit from one-bit correction. It is not a decoder, does not handle framing or interleaving, and the unextended Hamming(7,4) code does not safely detect all multi-bit errors. For double-error detection use an extended code such as Hamming(8,4); for burst errors use an appropriate stronger code or interleaving.
