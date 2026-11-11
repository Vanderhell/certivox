# Certivox API reference

The public surface contains 14 callable C APIs. The declarations in `include/certivox/` define the ABI; these notes describe the behavior observed in the current implementations and direct tests. CRCs detect accidental errors and are not cryptographic authentication.

## CRC operations

### `cv_crc8_compute`

Computes CRC-8/SMBus PEC: width 8, polynomial `0x07`, init `0`, xorout `0`, non-reflected. Accepts any readable byte buffer; `data == NULL` is accepted only when `length == 0`. Returns `false` for invalid pointers and leaves `*crc` unchanged on failure. Direct known-answer, empty/invalid-input, output-preservation and 100,000-case table-oracle tests are enabled.

### `cv_crc16_compute`

Computes CRC-16/MODBUS: width 16, reflected polynomial `0xA001` (normal form `0x8005`), init `0xFFFF`, xorout `0`. Pointer and failure contract matches CRC-8. Direct known-answer, empty/invalid-input, output-preservation and 100,000-case table-oracle tests are enabled.

### `cv_crc24_compute`

Computes CRC-24/OpenPGP: width 24, polynomial `0x864CFB`, init `0xB704CE`, xorout `0`, non-reflected. The result is in the low 24 bits of the output. Pointer and failure contract matches CRC-8. Direct known-answer, empty/invalid-input, output-preservation and 100,000-case table-oracle tests are enabled.

### `cv_crc32_compute`

Computes CRC-32/ISO-HDLC: reflected polynomial `0xEDB88320`, init/xorout `UINT32_MAX`. Pointer and failure contract matches CRC-8. Direct known-answer, streaming equivalence, empty/invalid-input, output-preservation and 100,000-case table-oracle tests are enabled.

### `cv_crc64_compute`

Computes CRC-64/ECMA-182: polynomial `0x42F0E1EBA9EA3693`, init `0`, xorout `0`, non-reflected. Pointer and failure contract matches CRC-8. Direct known-answer, empty/invalid-input, output-preservation and 100,000-case table-oracle tests are enabled.

### `cv_crc_update_byte_u32`

Advances a caller-owned raw CRC-32 state by one byte using a reflected polynomial. It does not apply initialization or final XOR and accepts every 32-bit state, byte, and polynomial bit pattern; it has no failure return. Differential tests cover 100,000 deterministic state/byte/polynomial combinations with a lookup-table reference.

### `cv_crc_combine_u32`

Combines finalized CRC-32/ISO-HDLC values as `CRC(A || B)` using `CRC(A)`, `CRC(B)`, and the byte length of `B` (`uint32_t`). For an empty suffix, `CRC(B)` is zero and the result is `CRC(A)`. There is no status return to reject inconsistent CRC/length tuples. Tests compare 100,000 deterministic random segment pairs to a direct table-driven CRC of the concatenated bytes, include a 65,536-byte suffix and a `UINT32_MAX` length reference vector from zlib 1.3.2.

### `cv_crc_reflect_poly_u32`

Reflects the low `width` polynomial bits; the implicit `x^width` term is not supplied. Valid widths are 1..32 and no polynomial bits may be set above `width`. Returns `false` for invalid width, polynomial or null output; output is preserved on failure. Tests include 100,000 deterministic valid inputs, invalid boundaries and exhaustive widths 1..8.

## Error-correction operations

### Hamming(7,4): `cv_hamming74_encode`, `cv_hamming74_decode`

The encoder accepts nibbles 0..15 and emits even-parity code bits in the documented positions. Invalid nibble or null output returns failure without changing output. The decoder rejects bit 7 and null output; it corrects the syndrome-indicated bit and reports clean/nonzero syndrome. Single-bit correction is guaranteed. Multiple-bit errors are not reliably detected; exhaustive host coverage shows two-bit errors can be miscorrected as a different nibble. Decoder output is preserved only for invalid input.

### Extended Hamming(8,4): `cv_hamming84_encode`, `cv_hamming84_decode`

The encoder accepts nibbles 0..15, adds overall even parity, and preserves output on invalid input. The decoder returns `INVALID` for a null output pointer, corrects any single-bit error, reports all double-bit errors as uncorrectable and preserves the caller's output for uncorrectable input. Three or more errors are outside the guarantee and may be misclassified.

### SECDED(39,32): `cv_secded_parity_u32`, `cv_secded_check_u32`

The parity API maps 32 data bits into positions 1..38, stores six positional parity bits in check bits 0..5 and overall parity in bit 6. Check API returns `INVALID` for a null output pointer or reserved check bit 7, corrects any single-bit error across the 39-bit codeword, detects any double-bit error, and preserves the caller's output for reserved-bit or uncorrectable input. Three or more bit errors are outside the SECDED guarantee and may be miscorrected or appear clean. These functions do not protect against intentional modification.

## Direct test mapping

| API | Direct test |
|---|---|
| `cv_crc8_compute` | `tests/test_crc8_compute.c` |
| `cv_crc16_compute` | `tests/test_crc16_compute.c` |
| `cv_crc24_compute` | `tests/test_crc24_compute.c` |
| `cv_crc32_compute` | `tests/test_crc32_compute.c` |
| `cv_crc64_compute` | `tests/test_crc64_compute.c` |
| `cv_crc_update_byte_u32` | `tests/test_crc_update_byte_u32.c` |
| `cv_crc_combine_u32` | `tests/test_crc_combine_u32.c` |
| `cv_crc_reflect_poly_u32` | `tests/test_crc_reflect_poly_u32.c` |
| `cv_hamming74_encode` / `cv_hamming74_decode` | `tests/test_hamming74_encode.c` / `tests/test_hamming74_decode.c` |
| `cv_hamming84_encode` / `cv_hamming84_decode` | `tests/test_hamming84_encode.c` / `tests/test_hamming84_decode.c` |
| `cv_secded_parity_u32` / `cv_secded_check_u32` | `tests/test_secded_parity_u32.c` / `tests/test_secded_check_u32.c` |

`tests/test_audit_differential.c` independently exercises all 14 APIs. Checks use explicit `CHECK` logic, not `assert()`, so they remain active with `NDEBUG`. Matrix results and limits are in `docs/TESTING.md`.

All 14 APIs were also called by the current ESP32-S3 validation firmware, which printed `CERTIVOX_ALL14_PASS`. This is representative hardware validation, not exhaustive enumeration of every possible input; hardware configuration and measurements are documented in `docs/PERFORMANCE.md`.
