# Architecture

Certivox is a freestanding-friendly C99 static library with no global mutable
state and no heap allocation.

The API is divided into two independent functional groups:

- CRC: fixed profiles for CRC-8, CRC-16/MODBUS, CRC-24/OpenPGP, CRC-32/ISO-HDLC
  and CRC-64/ECMA-182, plus streaming byte update, CRC-32 concatenation and
  polynomial reflection.
- ECC: Hamming(7,4), extended Hamming(8,4), and extended Hamming(39,32)
  SECDED parity/check operations.

Each source file contains one public primitive family. The CMake target keeps
all sources together for convenience, while consumers can compile only the
required source files. Functions write through caller-owned output pointers,
return explicit status where validation can fail, and do not retain state
between calls.

The `cv_` prefix is the standalone product API. It maps from the source
catalog's `pc_` symbols without changing numerical algorithms or contracts.
