# Certivox family guide

## What this family solves

**Encoding and integrity.** CRC profiles, streaming CRC helpers, Hamming codes and SECDED error detection.

This guide describes the complete public family as an independent embedded-C product. Each primitive remains separately linkable; select only the headers and source units required by the application.

## When to use this family

Use Certivox when the application needs the operations listed below and requires deterministic behavior, explicit caller-owned memory, and no mandatory operating-system services.

## Family at a glance

```mermaid
flowchart LR
    A[Application input] --> B[Validate representation and bounds]
    B --> C[Select one public primitive]
    C --> D[Check status or output contract]
    D --> E[Use result with documented units]
```

## Primitive map

| Public header | Symbol | Purpose and contract | Validation source |
|---|---|---|---|
| `include/certivox/cv_crc_combine_u32.h` | `cv_crc_combine_u32` | Combine finalized CRC-32/ISO-HDLC values for A and B into CRC(A // B). length_b is B's byte length; for an empty B its CRC must be zero. | tests/test_crc_combine_u32.c |
| `include/certivox/cv_crc_reflect_poly_u32.h` | `cv_crc_reflect_poly_u32` | Reflect the low `width` bits of a CRC polynomial (implicit x^width term omitted). Width must be 1..32 and polynomial must fit that width. | tests/test_crc_reflect_poly_u32.c |
| `include/certivox/cv_crc_update_byte_u32.h` | `cv_crc_update_byte_u32` | Advance a reflected raw 32-bit CRC state by one byte. The polynomial is supplied in reflected form; callers own init and final xor conventions. | tests/test_crc_update_byte_u32.c |
| `include/certivox/cv_crc16_compute.h` | `cv_crc16_compute` | The public header contains the authoritative signature and input/output contract. | tests/test_crc16_compute.c |
| `include/certivox/cv_crc24_compute.h` | `cv_crc24_compute` | The public header contains the authoritative signature and input/output contract. | tests/test_crc24_compute.c |
| `include/certivox/cv_crc32_compute.h` | `cv_crc32_compute` | The public header contains the authoritative signature and input/output contract. | tests/test_crc32_compute.c |
| `include/certivox/cv_crc64_compute.h` | `cv_crc64_compute` | The public header contains the authoritative signature and input/output contract. | tests/test_crc64_compute.c |
| `include/certivox/cv_crc8_compute.h` | `cv_crc8_compute` | The public header contains the authoritative signature and input/output contract. | tests/test_crc8_compute.c |
| `include/certivox/cv_hamming74_decode.h` | `cv_hamming74_decode` | Decode even-parity Hamming(7,4). A nonzero syndrome flips its indicated bit and is reported as such; only a single-bit error is guaranteed corrected. | tests/test_hamming74_decode.c |
| `include/certivox/cv_hamming74_encode.h` | `cv_hamming74_encode` | Encode nibble d3..d0 into even-parity Hamming(7,4), positions numbered 1..7 in code bits 0..6; parity occupies positions 1,2,4. | tests/test_hamming74_decode.c |
| `include/certivox/cv_hamming84_decode.h` | `cv_hamming84_decode` | Decode extended even-parity Hamming(8,4): correct single errors, detect double errors. Output is unchanged on invalid or uncorrectable input. | tests/test_hamming84_decode.c |
| `include/certivox/cv_hamming84_encode.h` | `cv_hamming84_encode` | Encode nibble with Hamming(8,4): positions 1..7 match Hamming(7,4), bit 7 is overall even parity. | tests/test_hamming84_decode.c |
| `include/certivox/cv_secded_check_u32.h` | `cv_secded_check_u32` | Check/correct extended Hamming(39,32). Check bits 0..5 are positional parity, bit 6 is overall parity; bit 7 is invalid. Output is unchanged for invalid or uncorrectable inputs. | tests/test_integration.c |
| `include/certivox/cv_secded_parity_u32.h` | `cv_secded_parity_u32` | Return seven check bits for extended even-parity Hamming(39,32): bits 0..5 are Hamming parity for positions 1,2,4,8,16,32; bit 6 is overall parity. | tests/test_integration.c |

## Selection guidance

- Choose the primitive whose width, representation, rounding, saturation, wrapping, and error contract match the application.
- Do not substitute a similarly named function without checking the public header.
- Allocate state and buffers in the caller; respect documented aliasing and output-preservation rules.
- Treat entries marked without a validation source as requiring additional product-level tests before release.

## Integration checklist

1. Include only the required public header.
2. Compile the matching source translation unit or use the product CMake target.
3. Check every boolean/status return before consuming an output.
4. Add application-specific boundary tests for units, widths, invalid inputs, and overflow behavior.
5. Run the documented host tests before target integration.

## Complexity and footprint

The product is split into independent translation units. Exact flash, RAM, stack, and cycle costs depend on the selected functions, compiler, optimization, and MCU; unmeasured values remain explicitly marked PENDING in the testing documentation.

## Validation boundary

The API reference links each public entry to its implementation and available validation source. Hardware status is reported only when reproducible target evidence exists.
