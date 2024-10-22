# simd-vec

C library that provides a dynamic array of float values, backed by SIMD AVX Intel x86 instructions. This library is a PoC on how to speed up numeric operations using this compiler intrinsic functions.

## Implementation

The idea of the library is to provide a float-specific dynamic array that uses AVX2 to store 32 bit single precision floating points on 256 bit registers, allowing to make parallel operations on all values in this vectorized registers. Theoretically, and ideal implementation would provide 4x the performance since we can work upon 4 32 bit values at once.

## Running the project

In the examples folders, there are examples for the simd-vec library. To run all examples `cd` into examples and run `make`.

To use this library as standalone instead, the AVX2 extension must be available for the compiler (in gcc, its possible to compile with the **-mavx** flag)
