# Architecture

Certivox is a freestanding-friendly C99 static library with no global mutable
state and no heap allocation.

The API is divided into two functional groups:

- CRC profiles for CRC-8, CRC-16/MODBUS, CRC-24/OpenPGP, CRC-32/ISO-HDLC
  and CRC-64/ECMA-182, plus streaming byte update, CRC-32 concatenation and
  polynomial reflection.
- Error-correction operations for Hamming(7,4), extended Hamming(8,4), and
  extended Hamming(39,32) SECDED parity/check.

Each source file implements one public operation. The CMake target combines
all source files for convenience, while direct integrations can compile only
the required source unit. Calls use caller-owned buffers and outputs, return
explicit status where validation can fail, and retain no state between calls.
