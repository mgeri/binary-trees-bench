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

I decided to make my own test to better understand the behavior of C Gcc, Java and Go using the same *strategy* and take them easy to understand and to best compare the benchmarks results.

I wrote 4 kind of tests for each language (Go, Java, C):

1. **Binarytrees**: based on single thread (no concurrency) and default language common allocator: *malloc* and *free* for C gcc, *new* for Java and struct allocation for Go.
2. **Binarytrees2**: It uses concurrency. C Gcc uses pthread library and create one thread for each binary trees test, Java use an executor thread pool and Go uses go routines.
3. **Binarytrees3**: like Binarytree but each test use its own pool of Nodes. C and Go uses a contiguous memory array of Nodes, Java use an array of reference (so as expected it could not have a great benefit from it)
4. **Binarytrees4**: join Binarytrees2 and Binarytrees3, i.e. it uses concurrency and a memory pool of Nodes. 

Note: there is also a Binarytrees4_1 for Go, which is the same test as Binarytrees4 where each node instead of having a pointer for left and right nodes, has a node "identifier": this should help Go GC because of less references to analyze.


## Hardware, OS and compiler/jvm version

I used my own Dell notebook:

Dell 5480
Intel(R) Core(TM) i7-7820HQ CPU @ 2.90GHz
32GiB System memory

Fedora 32 OS

Gcc 10.1, OpenJDK 14, Go 1.4

## How to build and run benchmarks

* install Gcc, Java and Go
* install hyperfine (see https://github.com/sharkdp/hyperfine)
* run `make` on main folder
* run `run-bench.sh` 

The output is `bench-all.md` file with hyperfine benchmarks results.

## Benchmarks results

| Command | Mean [s] | Min [s] | Max [s] | Relative |
|:---|---:|---:|---:|---:|
| `gcc/binarytrees.gcc_run 21` | 13.628 ± 0.235 | 13.462 | 13.794 | 27.18 ± 1.08 |
| `gcc/binarytrees2.gcc_run 21` | 5.050 ± 0.165 | 4.934 | 5.166 | 10.07 ± 0.49 |
| `gcc/binarytrees3.gcc_run 21` | 1.844 ± 0.014 | 1.835 | 1.854 | 3.68 ± 0.13 |
| `gcc/binarytrees4.gcc_run 21` | 0.501 ± 0.018 | 0.476 | 0.525 | 1.00 |
| `java -cp java binarytrees 21` | 4.360 ± 0.091 | 4.295 | 4.424 | 8.69 ± 0.36 |
| `java -cp java binarytrees2 21` | 2.170 ± 0.043 | 2.140 | 2.200 | 4.33 ± 0.18 |
| `java -cp java binarytrees3 21` | 4.380 ± 0.111 | 4.302 | 4.458 | 8.73 ± 0.38 |
| `java -cp java binarytrees4 21` | 1.716 ± 0.036 | 1.690 | 1.741 | 3.42 ± 0.14 |
| `go/binarytrees.go_run 21` | 24.677 ± 0.056 | 24.637 | 24.717 | 49.21 ± 1.77 |
| `go/binarytrees2.go_run 21` | 11.353 ± 0.304 | 11.138 | 11.568 | 22.64 ± 1.01 |
| `go/binarytrees3.go_run 21` | 4.133 ± 0.149 | 4.028 | 4.238 | 8.24 ± 0.42 |
| `go/binarytrees4.go_run 21` | 1.224 ± 0.044 | 1.193 | 1.255 | 2.44 ± 0.12 |
| `go/binarytrees4_1.go_run 21` | 1.241 ± 0.042 | 1.211 | 1.271 | 2.48 ± 0.12 |

Summary
```
  'gcc/binarytrees4.gcc_run 21' ran
    2.44 ± 0.12 times faster than 'go/binarytrees4.go_run 21'
    2.48 ± 0.12 times faster than 'go/binarytrees4_1.go_run 21'
    3.42 ± 0.14 times faster than 'java -cp java binarytrees4 21'
    3.68 ± 0.13 times faster than 'gcc/binarytrees3.gcc_run 21'
    4.33 ± 0.18 times faster than 'java -cp java binarytrees2 21'
    8.24 ± 0.42 times faster than 'go/binarytrees3.go_run 21'
    8.69 ± 0.36 times faster than 'java -cp java binarytrees 21'
    8.73 ± 0.38 times faster than 'java -cp java binarytrees3 21'
   10.07 ± 0.49 times faster than 'gcc/binarytrees2.gcc_run 21'
   22.64 ± 1.01 times faster than 'go/binarytrees2.go_run 21'
   27.18 ± 1.08 times faster than 'gcc/binarytrees.gcc_run 21'
   49.21 ± 1.77 times faster than 'go/binarytrees.go_run 21'
```

## Considerations

Without any allocation strategy, Go and Gcc are worst then java. Do you expected it?

But why Java is so much faster?

Well, Java, unlike Go, uses generational garbage collector, which has 2 arenas: one for young (newly allocated) objects (called nursery) and one for old objects. Generational garbage collector are based on empirical observation: 

*It has been observed that most objects die young.<br/>
Generational garbage collector allocates objects in nursery.<br/> 
Most collections only collect objects in nursery.<br/> 
Objects that survived collections in nursery are moved to the second arena for old objects, which is collected at a much lower rate.*

Generational garbage collectors have overhead because they have to copy objects in memory and update references between objects when that happens. 
On the other hand they can also compact memory, improving caching and they scan a smaller number of objects during each collection.

Go garbage collector is a mark-and-sweep collector which has to scan all allocated objects during every collection. Take in account that Go garbage collector has been designed to archive those goals:

* reducing as much as possible in the two phases when the program is stopped, also called “stop the world.”
* a cycle of the garbage collector that takes less than 10ms.
* the garbage collection cycle should not take more than 25% of the CPU.

And it works very well in parallel request-response scenario.

In this particular benchmark there are many Node objects and they never die, so they are promoted to rarely collected arena for old objects and each collection is cheaper because it only looks at a small number of recently allocated Node objects.

*Note*: nowadays GC are very complex because many years of studying and evolution and although from what I wrote they seems "simple", there are not! :) 

If we use a simple allocation strategy and optimization, the things change and they seem like what one would expect: C Gcc is the fastest... no doubts! Then Go seems to work much better, that is because the possibility to allocate a contiguous array of Nodes in memory.

## Conclusion

* Study each programming language with its own characteristics. There is no best one, but only someone which could better fit your needs (performance, memory, concurrency, maintainability...)  
* Always profile your programs to understand how it works and the performance bottleneck
* Memory allocation and task specific allocation strategy is very important in data processing task and may help you boost your program performance regardless of the language used.


And finally... don't trust the benchmarks numbers before understanding how they are produced! ;)
