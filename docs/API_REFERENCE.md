# Certivox API reference

This reference explains every public header in the Certivox product. It is generated from the current headers and test sources; the header remains authoritative when a detail conflicts.

## Reading the entries
Each entry states the public symbol, implementation unit, contract, edge/failure behavior and a call extracted from the validating test source. The examples are evidence-oriented; adapt buffer ownership and numeric units to the application.

## include/certivox/cv_crc_combine_u32.h

**Public symbol:** cv_crc_combine_u32

**Implementation unit:** src/cv_crc_combine_u32.c

**Purpose and contract:** Combine finalized CRC-32/ISO-HDLC values for A and B into CRC(A || B). length_b is B's byte length; for an empty B its CRC must be zero.

**Validation source:** tests/test_crc_combine_u32.c

**Test-derived call:**
```c
cv_crc_combine_u32(crc_a,crc_b,(uint32_t)(sizeof(b)-1u))==expected);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc_reflect_poly_u32.h

**Public symbol:** cv_crc_reflect_poly_u32

**Implementation unit:** src/cv_crc_reflect_poly_u32.c

**Purpose and contract:** Reflect the low `width` bits of a CRC polynomial (implicit x^width term omitted). Width must be 1..32 and polynomial must fit that width.

**Validation source:** tests/test_crc_reflect_poly_u32.c

**Test-derived call:**
```c
cv_crc_reflect_poly_u32(0x07u,8u,&out)&&out==0xE0u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc_update_byte_u32.h

**Public symbol:** cv_crc_update_byte_u32

**Implementation unit:** src/cv_crc_update_byte_u32.c

**Purpose and contract:** Advance a reflected raw 32-bit CRC state by one byte. The polynomial is supplied in reflected form; callers own init and final xor conventions.

**Validation source:** tests/test_crc_update_byte_u32.c

**Test-derived call:**
```c
cv_crc_update_byte_u32(crc,text[i],UINT32_C(0xEDB88320\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc16_compute.h

**Public symbol:** cv_crc16_compute

**Implementation unit:** src/cv_crc16_compute.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** tests/test_crc16_compute.c

**Test-derived call:**
```c
cv_crc16_compute(check,sizeof(check)-1u,&crc)&&crc==0x4B37u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc24_compute.h

**Public symbol:** cv_crc24_compute

**Implementation unit:** src/cv_crc24_compute.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** tests/test_crc24_compute.c

**Test-derived call:**
```c
cv_crc24_compute(check,sizeof(check)-1u,&crc)&&crc==0x21CF02u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc32_compute.h

**Public symbol:** cv_crc32_compute

**Implementation unit:** src/cv_crc32_compute.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** tests/test_crc32_compute.c

**Test-derived call:**
```c
cv_crc32_compute(check,sizeof(check)-1u,&crc)&&crc==UINT32_C(0xCBF43926\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc64_compute.h

**Public symbol:** cv_crc64_compute

**Implementation unit:** src/cv_crc64_compute.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** tests/test_crc64_compute.c

**Test-derived call:**
```c
cv_crc64_compute(check,sizeof(check)-1u,&crc)&&crc==UINT64_C(0x6C40DF5F0B497347\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_crc8_compute.h

**Public symbol:** cv_crc8_compute

**Implementation unit:** src/cv_crc8_compute.c

**Purpose and contract:** The public header contains the authoritative signature and input/output contract.

**Validation source:** tests/test_crc8_compute.c

**Test-derived call:**
```c
cv_crc8_compute(check,sizeof(check)-1u,&crc)&&crc==0xF4u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_hamming74_decode.h

**Public symbol:** cv_hamming74_decode

**Implementation unit:** src/cv_hamming74_decode.c

**Purpose and contract:** Decode even-parity Hamming(7,4). A nonzero syndrome flips its indicated bit and is reported as such; only a single-bit error is guaranteed corrected.

**Validation source:** tests/test_hamming74_decode.c

**Test-derived call:**
```c
cv_hamming74_decode(code,&decoded)==CV_HAMMING74_SYNDROME_ZERO&&decoded==n);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_hamming74_encode.h

**Public symbol:** cv_hamming74_encode

**Implementation unit:** src/cv_hamming74_encode.c

**Purpose and contract:** Encode nibble d3..d0 into even-parity Hamming(7,4), positions numbered 1..7 in code bits 0..6; parity occupies positions 1,2,4.

**Validation source:** tests/test_hamming74_decode.c

**Test-derived call:**
```c
cv_hamming74_encode(n,&code\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_hamming84_decode.h

**Public symbol:** cv_hamming84_decode

**Implementation unit:** src/cv_hamming84_decode.c

**Purpose and contract:** Decode extended even-parity Hamming(8,4): correct single errors, detect double errors. Output is unchanged on invalid or uncorrectable input.

**Validation source:** tests/test_hamming84_decode.c

**Test-derived call:**
```c
cv_hamming84_decode(code,&out)==CV_HAMMING84_CLEAN&&out==nibble);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_hamming84_encode.h

**Public symbol:** cv_hamming84_encode

**Implementation unit:** src/cv_hamming84_encode.c

**Purpose and contract:** Encode nibble with Hamming(8,4): positions 1..7 match Hamming(7,4), bit 7 is overall even parity.

**Validation source:** tests/test_hamming84_decode.c

**Test-derived call:**
```c
cv_hamming84_encode(nibble,&code\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_secded_check_u32.h

**Public symbol:** cv_secded_check_u32

**Implementation unit:** src/cv_secded_check_u32.c

**Purpose and contract:** Check/correct extended Hamming(39,32). Check bits 0..5 are positional parity, bit 6 is overall parity; bit 7 is invalid. Output is unchanged for invalid or uncorrectable inputs.

**Validation source:** tests/test_integration.c

**Test-derived call:**
```c
cv_secded_check_u32(word, check_bits, &corrected) == CV_SECDED_CLEAN);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/certivox/cv_secded_parity_u32.h

**Public symbol:** cv_secded_parity_u32

**Implementation unit:** src/cv_secded_parity_u32.c

**Purpose and contract:** Return seven check bits for extended even-parity Hamming(39,32): bits 0..5 are Hamming parity for positions 1,2,4,8,16,32; bit 6 is overall parity.

**Validation source:** tests/test_integration.c

**Test-derived call:**
```c
cv_secded_parity_u32(word, &check_bits\);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

