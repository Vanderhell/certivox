# ESP32-S3 result: `cv_hamming74_decode`

## Purpose and category

Decodes an even-parity Hamming(7,4) word and corrects the syndrome-indicated bit. Category: compact communications error correction for short symbols on channels where single-bit errors dominate.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=130|build=20261029a`.
- Sketch footprint: 310,912 bytes flash and 22,192 bytes static RAM, including runtime, oracle, and harness; not primitive-only. Decoder itself uses scalar local state, with no heap or table.

## Tests and results

- PASS: all 128 seven-bit received words matched an independent nearest-codeword oracle; clean words report zero syndrome, and corrected words report nonzero syndrome.
- PASS: all 112 single-bit corruptions of the 16 codewords decode to their original payload.
- PASS: all 128 byte values with bit 7 set are rejected without changing output; null output is rejected.
- Exhaustively exercised 336 two-bit corruptions. All 336 decode to a different symbol, documenting the known miscorrection behavior; the decoder status does not report the error count.
- PASS: 1,000,000 deterministic clean/single-bit-error inputs checked against the oracle; checksum `194797568`.
- 10 batches of 100,000 calls: mean 79.90 cycles/call, range 79.89..79.90 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_hamming74_decode|PASS|exhaustive=128|corrected=112|invalid=128|two_bit=336|miscorrected=336|longrun=1000000|checksum=194797568|batch=100000|mean_cycles_x100=7990|min_cycles_x100=7989|max_cycles_x100=7990|cpu_mhz=240
```

## Use and limits

Use for short symbols on links where single-bit errors are expected and both endpoints share the exact bit ordering. The ordinary Hamming(7,4) code cannot reliably detect double-bit errors: this exhaustive device run observed all 336 tested two-bit corruptions miscorrect as another valid payload. Use extended Hamming(8,4) for SECDED, or combine with an outer integrity check; this decoder is not a security check.
