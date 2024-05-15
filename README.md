# FasterBitset - Bitset Benchmarks for Performance

<p>
<img alt="License" src="https://img.shields.io/github/license/zhang-stephen/FasterBitset?style=flat-square">
<img alt="Language" src="https://img.shields.io/badge/C%2B%2B20-blue?style=flat-square&logo=C%2B%2B">
<img alt="Platform" src="https://img.shields.io/badge/ARMv8--A-black?style=flat-square&logo=arm&logoColor=blue">
<img alt="platform" src='https://img.shields.io/badge/mac%20os-000000?style=flat-square&logo=apple&logoColor=F0F0F0'>
</p>

# Preface
It's a simple repo for PoC(Proving of Concept), for bitset performance on ARMv8-A or ARMv9-A.

It's was inspired by a problem in my daily works: There is a real-time, performance-sensitive module, which uses `std::bitset` to hold some flags(~300 bits). The profiling results shows that most CPU cycles were comsumed for bitset operations. I have to do something to improve the performance of bitset.

Technically, the data structure, `std::bitset`, is not a STL container, even though it use array of `unsigned long` to store all of bits:

- the iterator is not supported in `std::bitset`(range-based operations are not allowed),
- it does not allow users to customize memory allocator,

*The storage of bits is implementation-defined, GCC uses array of `unsigned long`, see [here](https://github.com/gcc-mirror/gcc/blob/5609d77e683944439fae38323ecabc44a1eb4671/libstdc%2B%2B-v3/include/std/bitset#L87).*

Then we cannot access the storage memory directly in the same way as other STL containers, e.g. `std::vector`. And some hacking methods on the underlying memory are unavailable.

And for the `std::bitset` class itself, if you access a bit by `operator[]`, you will get a instance of a proxy class, which could be implicitlt converted as `bool`, instead of a real boolean value. I think it was designed for the possible assignemnt by the `operator[]`. The proxy class, named as `reference` in the `std::bitset` is implemented [here](https://github.com/gcc-mirror/gcc/blob/5609d77e683944439fae38323ecabc44a1eb4671/libstdc%2B%2B-v3/include/std/bitset#L860) in GCC.

Is there any performance cost for the complicated designs? I have no idea but I should find it out. Then this repo was designed to finish following issues.

# Performance Issues

## Simplified `bitset`
The first question was raised in the last section --- is there any performance cost for the complicated designs? The anwser from my aspect is 'YES'. But it still need to be proved by benchmark.

There would be a simplified implementation of bitset, with the C-Style bitwise manipulation, and its benchmark.

The first C-Style bitwise manipulation is implemented in [bitset.cstyle.hh](./src/bitset.cstyle.hh).

## SIMD optimization
The optimization target is a kind of ARMv9-A SoC. As an application processor, A-Profile CPUs of ARM support some ISA extension for SIMD(Single Instruction, Multiple Data) operations, e.g. ARM Neon & ARM SVE2.

For this situation, most of bitset operation, such as range-based operations(set or reset, shift & etc), could be implemented by SIMD intrinsics.

*The hand-coded assmbler might be more effeient than intrinsics, but I want better readablity.*

By the way, there are 3 kinds of SIMD extension from ARM, Helium, Neon, SVE & SVE2:

- Helium was introduced from ARMv8-M, it's M-Profile Vector Extension(MVE),
- Neon was introduced from ARMv7, both for A-Profile and R-Profile. And it is supported by ARMv8 and newer CPU core in the default.
- SVE(Scalable Vector Extension) was introduced by ARMv8, and SVE2 was introduced by ARMv9, they are supported in the ARMv9 in the default, but SVE is optional for ARMv8.

The vector length in Neon is 128 bits, it's fixed. But the length of vector for SVE is scalable, it supports 128 bits to 2048 bits, specified by the SoC vendor.

Well, I have introduced the SIMD extension for ARM. And you might have this question: why not to relying on the vectorizing optimization from compiler?

The answer is the vectorization would be not stable enough for bitset. According to the manual of [LLVM](https://llvm.org/docs/Vectorizers.html) and [GCC](https://gcc.gnu.org/onlinedocs/gcc-14.1.0/gcc/Optimize-Options.html#index-ftree-vectorize), the vectorization should be applied implicitly with `-O3` optimization level. But the compiler would not optimize the range-based set operation of `std::bitset`, I guess it is due to the range cannot be determined in the compilie-time.

On the other hand, if the range could be determined in the compile-time, the range-based operation would be replaced by several of assignment instructions, because the `std::bitset` is marked as `constexpr` after C++11.

So the 2nd idea is use SIMD intrinsics to implement a specialized `bistset`, it is designed for ARMv8-A and ARMv9-A only, and run benchmark for this implementation.

About this idea, I would implement the `bitset` based on Neon intrinsics, because I have no device support the ARMv9-A. This repo is created on Macbook Pro 2021 that its CPU is based on ARMv8-A, that means Neon is supported only.

As for ARMv9-A, I will try test with QEMU, but it is out of planning for now.

## Other Requirement

How to split the bitset into vector of continous 1 or 0? Image that, there is a bitset:

```cpp
0b1111'0000'1110'1011
```

I want to split it to be following:
```cpp
0b1111, 0b0000, 0b111, 0b0, 0b1, 0b0, 0b11
```

Furthermore what to do if I want to keep subsequences which is consisted by 1 or 0? What to do if I want to covert the splitted subsequences to range specifications like following?

```cpp
// all-0 subsequences were filtered out
{12, 4}, {9, 3}, {3, 1}, {0, 2} // pattern:  {start, length}
```

The splitting operation bit-by-bit is a heavy job for CPU, is there some SIMD operation for this request? I am not sure, but I will take a try.

# About the Repo
As the beginning of the README itself, it would be implemented by C++20. And all benchmark would be done with `-O3` optimization level(`CMAKE_BUILD_TYPE` would be set to `Release`).

Dependencies:
- CMake 3.27
- Git 2.34
- LLVM 16 and later
- GCC 11.4 and later(not support now)

Third-party libs:
- vcpkg 2024.03.25, as default third-party lib manager
- gtest 1.13 for unit test
- google benchmark 1.8.3 for benchmark test
- STL from compilers

Supported platform:
- ARMv8-A SoC, e.g. Apple Silicon M1 pro, BCM2711(RaspberryPi 4B, not tested yet)
<!-- - ARMv9-A SoC, in the planning
- x86-64, with Neon2SSE, out of the planning -->

Building:
Switch to the root path of the repo, then run the following commands. And executable binary would be found in `build/test/ut` or `build/test/benchmark`. The `src/` is header-only implementation.

```shell
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j <n>
```

# Benchmark Results
*To be finished...*

# References
1. ARM Documents Official
    - [Introduction to Neon](https://developer.arm.com/Architectures/Neon)
    - [Introduction to SVE](https://developer.arm.com/Architectures/Scalable%20Vector%20Extensions)
    - [ARM Intrinsics Sets](https://developer.arm.com/architectures/instruction-sets/intrinsics/)
2. [Critical look for ARM SVE2 - zingaburga's gist](https://gist.github.com/zingaburga/805669eb891c820bd220418ee3f0d6bd)
3. [`-ftree-vectorize` option of GCC 14](https://gcc.gnu.org/onlinedocs/gcc-14.1.0/gcc/Optimize-Options.html#index-ftree-vectorize)
