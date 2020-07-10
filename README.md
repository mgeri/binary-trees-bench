# Benchmark Game Binary-Trees: under the hood

The famous [The Computer Language Benchmarks Game](https://benchmarksgame-team.pages.debian.net/benchmarksgame/) site offers practical and excellent synthetic benchmark test for many Programming Language. 

Go Language is one language I like to study my spare timing... and yes... when I saw the [Binary Trees benchmarks](https://benchmarksgame-team.pages.debian.net/benchmarksgame/performance/binarytrees.html) I was surprised that it so slower that several other language (native and VM).

Binary Trees test is memory-intensive task and if you profile it the problem i quite clear: memory allocation is bottleneck.

But how is possible that Java is medially 2 times slower than C gcc and Go 6 time slower that C gcc?

First of all there are some important notes int the  [Binary-Trees test description](https://benchmarksgame-team.pages.debian.net/benchmarksgame/description/binarytrees.html#binarytrees): 

* When possible, use default GC; otherwise use per node allocation or use a **library memory pool**.
* As a practical matter, **the myriad ways to tune GC will not be accepted**.
* As a practical matter, **the myriad ways to custom allocate memory will not be accepted**.
* Please **don't implement your own custom "arena" or "memory pool" or "free list"** - they will not be accepted

Looking at the source code you can see the faster native language like **C gcc** make use of **library memory pool** (arena pool), **Java** and **Go** do not use any pool (so it's based on it's own GC) because the **Rules** of the Benchmark Game site.

## My benchmarks 

I decided to make my own test to better understand the behavior of C Gcc, Java and Go using the same *strategy* while compering the benchmarks results.

I wrote 4 kind of tests for each language (Go, Java, C):

1. Binarytrees: based on single thread (no concurrency) and default language common allocator: *malloc* and *free* for C gcc, *new* for Java and struct allocation for Go.
2. Binarytrees2: It uses concurrency. C Gcc uses pthread library and create one thread for each binary trees test, Java use an executor thread pool and Go uses go routines.
3. Binarytrees3: like Binarytree but each test use it's own pool of Nodes. C and Go uses a contiguous memory array of Nodes, Java use an array of reference (so as expected it could not have a great benefit from it)
4. Binarytrees4: join Binarytrees2 and Binarytrees3, i.e. it uses concurrency and a memory pool of Nodes. 

Note: for Go there is also a Binarytrees4_1 which is the same test of Binarytrees4 but each node instead of having a pointer for left and right nodes, ha a node "identifier": this should help Go GC because it hae less references to analyze.


## Hardware, OS and compiler/jvm version

I used my own Dell notebook:

Dell 5480
Intel(R) Core(TM) i7-7820HQ CPU @ 2.90GHz
32GiB System memory

OS Fedora 32 OS

With Gcc 10.1, OpenJDK 14, Go 1.4.

## How to build and run benchmarks

* install Gcc, Java and Go
* install hyperfine (see https://github.com/sharkdp/hyperfine)
* run `make` on main folder
* run `run-bench.sh` 

The output is `bench-all.md` file with hyperfine benchmarks results.

## Benchmarks results


