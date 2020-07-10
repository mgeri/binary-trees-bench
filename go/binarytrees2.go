package main

import (
	"flag"
	"fmt"
	"runtime"
	"strconv"
	"sync"
)

var n = 0

type Node struct {
	left, right *Node
}

func bottomUpTree(depth int) *Node {
	if depth <= 0 {
		return &Node{}
	}
	return &Node{bottomUpTree(depth - 1), bottomUpTree(depth - 1)}
}

func (n *Node) itemCheck() int {
	if n.left == nil {
		return 1
	}
	return 1 + n.left.itemCheck() + n.right.itemCheck()
}

const minDepth = 4

func main() {
	fmt.Println("Available precessors ", runtime.NumCPU())

	runtime.GOMAXPROCS(runtime.NumCPU() + 2)

	flag.Parse()
	if flag.NArg() > 0 {
		n, _ = strconv.Atoi(flag.Arg(0))
	}

	maxDepth := n
	if minDepth+2 > n {
		maxDepth = minDepth + 2
	}
	stretchDepth := maxDepth + 1

	check_l := bottomUpTree(stretchDepth).itemCheck()
	fmt.Printf("stretch tree of depth %d\t check: %d\n", stretchDepth, check_l)

	longLivedTree := bottomUpTree(maxDepth)

	var wg sync.WaitGroup
	result := make([]string, maxDepth+1)

	for depth_l := minDepth; depth_l <= maxDepth; depth_l += 2 {
		wg.Add(1)
		go func(depth int, check int, r *string) {
			defer wg.Done()
			iterations := 1 << uint(maxDepth-depth+minDepth)
			check = 0

			for i := 1; i <= iterations; i++ {
				check += bottomUpTree(depth).itemCheck()
			}
			*r = fmt.Sprintf("%d\t trees of depth %d\t check: %d", iterations, depth, check)
		}(depth_l, check_l, &result[depth_l])
	}
	wg.Wait()
	for depth := minDepth; depth <= maxDepth; depth += 2 {
		fmt.Println(result[depth])
	}
	fmt.Printf("long lived tree of depth %d\t check: %d\n", maxDepth, longLivedTree.itemCheck())

}
