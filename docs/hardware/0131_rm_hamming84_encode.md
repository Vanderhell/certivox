# ESP32-S3 result: `cv_hamming84_encode`

## Purpose and category

Encodes a 4-bit payload using extended even-parity Hamming(8,4). Category: compact error-control coding for short communications fields; pair with a matching SECDED decoder.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=131|build=20261029a`.
- Sketch footprint: 310,904 bytes flash and 22,192 bytes static RAM including runtime, Hamming(7,4) dependency, oracle, and harness; not primitive-only. Production code uses scalar state, no heap, and no table.

## Tests and results

- PASS: exhaustive 0..255 inputs: valid nibbles 0..15 matched the independent position/parity oracle; remaining 240 inputs rejected without modifying output. Null output rejected.
- PASS: all 16 codewords have even parity; all 240 ordered distinct pairs have distance at least 4.
- PASS: all 128 single-bit corruptions produce odd overall parity. All 448 two-bit corruptions preserve even overall parity and remain at distance at least 2 from every valid codeword, so a correctly paired SECDED decoder can distinguish them from clean and single-bit-error cases.
- PASS: 1,000,000 deterministic encodes matched the oracle; checksum `1592399360`.
- 10 batches of 100,000 calls: mean 129.64 cycles/call, range 129.64..129.65 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_hamming84_encode|PASS|exhaustive=256|pairs=240|single=128|double=448|longrun=1000000|checksum=1592399360|batch=100000|mean_cycles_x100=12964|min_cycles_x100=12964|max_cycles_x100=12965|cpu_mhz=240
```

## Use and limits

Use where eight transmitted bits per four payload bits are acceptable and both endpoints use this exact position ordering. This is the encoder only; a matching SECDED decoder must interpret both the Hamming syndrome and overall parity. For large payloads or burst errors, consider interleaving or a stronger code. It is not cryptographic integrity protection.
