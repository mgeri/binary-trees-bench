package main

import (
	"flag"
	"fmt"
	"runtime"
	"strconv"
	"sync"
)

var n = 0

type NodeId int

type Node struct {
	left, right NodeId
}

type NodeBufferedAllocator struct {
	buffer []Node
	index  int
}

func itemCheck(id NodeId, allocator *NodeBufferedAllocator) int {
	n := allocator.buffer[int(id)]
	if n.left == NodeId(-1) {
		return 1
	}
	return 1 + itemCheck(n.left, allocator) + itemCheck(n.right, allocator)
}

func bottomUpTreeAllocator(depth int, allocator *NodeBufferedAllocator) NodeId {
	if depth <= 0 {
		return allocator.NewNode(-1, -1)
	}
	return allocator.NewNode(bottomUpTreeAllocator(depth-1, allocator), bottomUpTreeAllocator(depth-1, allocator))
}

func newNodeBufferedAllocator(depth int) *NodeBufferedAllocator {
	return &NodeBufferedAllocator{
		buffer: make([]Node, 1<<(depth+1)),
	}
}

func (a *NodeBufferedAllocator) NewNode(left, right NodeId) NodeId {
	id := a.index
	n := &a.buffer[id]
	n.left = left
	n.right = right
	a.index++
	return NodeId(id)
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

	stretchDepthAllocator := newNodeBufferedAllocator(stretchDepth)
	check_l := itemCheck(bottomUpTreeAllocator(stretchDepth, stretchDepthAllocator), stretchDepthAllocator)
	fmt.Printf("stretch tree of depth %d\t check: %d\n", stretchDepth, check_l)

	longLivedTreeAllocator := newNodeBufferedAllocator(maxDepth)
	longLivedTree := bottomUpTreeAllocator(maxDepth, longLivedTreeAllocator)

	var wg sync.WaitGroup
	result := make([]string, maxDepth+1)

	for depth_l := minDepth; depth_l <= maxDepth; depth_l += 2 {
		wg.Add(1)
		go func(depth int, check int, r *string) {
			defer wg.Done()
			iterations := 1 << uint(maxDepth-depth+minDepth)
			check = 0

			allocator := newNodeBufferedAllocator(depth)
			for i := 1; i <= iterations; i++ {
				check += itemCheck(bottomUpTreeAllocator(depth, allocator), allocator)
				allocator.index = 0
			}
			*r = fmt.Sprintf("%d\t trees of depth %d\t check: %d", iterations, depth, check)
		}(depth_l, check_l, &result[depth_l])
	}
	wg.Wait()
	for depth := minDepth; depth <= maxDepth; depth += 2 {
		fmt.Println(result[depth])
	}
	fmt.Printf("long lived tree of depth %d\t check: %d\n", maxDepth, itemCheck(longLivedTree, longLivedTreeAllocator))

}
