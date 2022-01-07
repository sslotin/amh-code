# Algorithms for Modern Hardware

This repository contains full examples and other associated code from https://en.algorithmica.org/hpc

The book is still unfinished, and my writing process is very slow and non-sequential — sometimes the "idea → code → benchmarks → article" pipeline may take 6 months or even more — so in this repository you can get a preview on a lot of interesting things that I haven't yet properly written up and published.

Things that have improved on the state-of-the-art:

- Many variants of [binary search](https://github.com/sslotin/amh-code/tree/main/binsearch), the [fastest one](https://github.com/sslotin/amh-code/blob/main/binsearch/bplus.cc) achieving ~15x speedup over `std::lower_bound` for small arrays (that fit in cache) and ~8x speedup for large arrays (>1e6).
- [Argmin at the speed of memory](https://github.com/sslotin/amh-code/blob/main/argmin/simdmin.cc).
- Implementation of [the Floyd-Warshall algorithm](https://github.com/sslotin/amh-code/tree/main/floyd) that is about 50x faster than the naive "for-for-for" algorithm.

Things that match current state-of-the-art:

- [A version of a segment tree](https://github.com/sslotin/amh-code/blob/main/segtree/refactor2.cc) that can compute prefix sums in ~2ns plus the time of the slowest memory read.
- (✓ published) [An implementation of GCD](https://en.algorithmica.org/hpc/analyzing-performance/gcd/) that works 2-3 faster than `std::gcd`.
- [Integer factorization](https://github.com/sslotin/amh-code/blob/main/factor/montgomery.cc) taking ~0.5ms per 60-bit integer.
- An algorithm for parsing series of integers ~2x faster than `scanf("%d")` does.
- An implementation of [BLAS-level matrix multiplication](https://github.com/sslotin/amh-code/blob/main/matmul/v6.cc) that can be expressed in [about 30 lines of C](https://gist.github.com/sslotin/fae39ea49a812732ae45db7b72f6a7ff).
- Various efficient [hash tables](https://github.com/sslotin/amh-code/tree/main/hash-tables).
- Efficient [FFT](https://github.com/sslotin/amh-code/tree/main/fft) and Karatsuba algorithm implementations.

Various benchmarks:

- Benchmarks for [branching and predication](https://github.com/sslotin/amh-code/tree/main/branching).
- Benchmarks for [RAM and CPU cache system](https://github.com/sslotin/amh-code/tree/main/cpu-cache).

At the implementation stage:

- Ordered Trees (apply the same technique as with binary searching, but with dynamically-allocated B-tree nodes)
- Range minimum queries (both static and dynamic)
- Filters (Bloom, cuckoo, xor, theoretical minimum)
- Dot product / logistic regression (newton's method, SIMD, quantization)
- Prime number sieves (blocking plus wheel)
- Sorting (speeding up quicksort and mergesort with SIMD and radix sort)
- Writing series of integers (SIMD + fast mod-10)
- Bitmaps (blocking, SIMD)

At the idea stage:

- String searching (SIMD-based strstr and rolling hashing)
- Using SIMD to speed up Pollard's algorithm (naive sqrt-parallelization)
- SIMD-based random number generation and hashing
