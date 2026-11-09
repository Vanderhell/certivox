# Certivox API reference

Complete public-header inventory generated from the current source tree. Headers remain authoritative.

## include/certivox/cv_crc_combine_u32.h

Public API: cv_crc_combine_u32

Contract: Combine finalized CRC-32/ISO-HDLC values for A and B into CRC(A || B). length_b is B's byte length; for an empty B its CRC must be zero.

Header path: include/certivox/cv_crc_combine_u32.h

## include/certivox/cv_crc_reflect_poly_u32.h

Public API: cv_crc_reflect_poly_u32

Contract: Reflect the low `width` bits of a CRC polynomial (implicit x^width term omitted). Width must be 1..32 and polynomial must fit that width.

Header path: include/certivox/cv_crc_reflect_poly_u32.h

## include/certivox/cv_crc_update_byte_u32.h

Public API: cv_crc_update_byte_u32

Contract: Advance a reflected raw 32-bit CRC state by one byte. The polynomial is supplied in reflected form; callers own init and final xor conventions.

Header path: include/certivox/cv_crc_update_byte_u32.h

## include/certivox/cv_crc16_compute.h

Public API: cv_crc16_compute

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/certivox/cv_crc16_compute.h

## include/certivox/cv_crc24_compute.h

Public API: cv_crc24_compute

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/certivox/cv_crc24_compute.h

## include/certivox/cv_crc32_compute.h

Public API: cv_crc32_compute

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/certivox/cv_crc32_compute.h

## include/certivox/cv_crc64_compute.h

Public API: cv_crc64_compute

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/certivox/cv_crc64_compute.h

## include/certivox/cv_crc8_compute.h

Public API: cv_crc8_compute

Contract: The public header contains the authoritative signature and input/output contract.

Header path: include/certivox/cv_crc8_compute.h

## include/certivox/cv_hamming74_decode.h

Public API: cv_hamming74_decode

Contract: Decode even-parity Hamming(7,4). A nonzero syndrome flips its indicated bit and is reported as such; only a single-bit error is guaranteed corrected.

Header path: include/certivox/cv_hamming74_decode.h

## include/certivox/cv_hamming74_encode.h

Public API: cv_hamming74_encode

Contract: Encode nibble d3..d0 into even-parity Hamming(7,4), positions numbered 1..7 in code bits 0..6; parity occupies positions 1,2,4.

Header path: include/certivox/cv_hamming74_encode.h

## include/certivox/cv_hamming84_decode.h

Public API: cv_hamming84_decode

Contract: Decode extended even-parity Hamming(8,4): correct single errors, detect double errors. Output is unchanged on invalid or uncorrectable input.

Header path: include/certivox/cv_hamming84_decode.h

## include/certivox/cv_hamming84_encode.h

Public API: cv_hamming84_encode

Contract: Encode nibble with Hamming(8,4): positions 1..7 match Hamming(7,4), bit 7 is overall even parity.

Header path: include/certivox/cv_hamming84_encode.h

## include/certivox/cv_secded_check_u32.h

Public API: cv_secded_check_u32

Contract: Check/correct extended Hamming(39,32). Check bits 0..5 are positional parity, bit 6 is overall parity; bit 7 is invalid. Output is unchanged for invalid or uncorrectable inputs.

Header path: include/certivox/cv_secded_check_u32.h

## include/certivox/cv_secded_parity_u32.h

Public API: cv_secded_parity_u32

Contract: Return seven check bits for extended even-parity Hamming(39,32): bits 0..5 are Hamming parity for positions 1,2,4,8,16,32; bit 6 is overall parity.

Header path: include/certivox/cv_secded_parity_u32.h
