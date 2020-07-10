
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

typedef struct tn
{
    struct tn *left;
    struct tn *right;
} treeNode;

typedef struct tnba
{
    treeNode *buffer;
    int index;
} treeNodeBufferedAllocator;

long ItemCheck(treeNode *tree)
{
    if (tree->left == NULL)
        return 1;
    else
        return 1 + ItemCheck(tree->left) + ItemCheck(tree->right);
} /* ItemCheck() */

treeNode *NewTreeNode(treeNode *left, treeNode *right, treeNodeBufferedAllocator *allocator)
{
    treeNode *new;

    new = &allocator->buffer[allocator->index];

    new->left = left;
    new->right = right;

    allocator->index++;

    return new;
} /* NewTreeNode() */

treeNode *BottomUpTreeWithAllocator(unsigned depth, treeNodeBufferedAllocator *allocator)
{
    if (depth > 0)
        return NewTreeNode(
            BottomUpTreeWithAllocator(depth - 1, allocator),
            BottomUpTreeWithAllocator(depth - 1, allocator),
            allocator);
    else
        return NewTreeNode(NULL, NULL, allocator);
} /* BottomUpTree() */

treeNodeBufferedAllocator *NewTreeNodeBufferedAllocator(int depth)
{
    treeNodeBufferedAllocator *a;

    a = (treeNodeBufferedAllocator *)malloc(sizeof(treeNodeBufferedAllocator));

    a->index = 0;
    a->buffer = (treeNode *)malloc(sizeof(treeNode) * (1 << (depth + 1)));

    return a;
}

void DeleteTreenodeBufferedAllocator(treeNodeBufferedAllocator *a)
{
    free(a->buffer);
    free(a);
}

int main(int argc, char *argv[])
{
    unsigned N, depth, minDepth, maxDepth, stretchDepth;
    treeNode *stretchTree, *longLivedTree, *tempTree;
    treeNodeBufferedAllocator *stretchTreeAlloc, *longLivedTreeAlloc, *treeAlloc;

    printf("Available processors %d\n", get_nprocs_conf());

    N = atol(argv[1]);

    minDepth = 4;

    if ((minDepth + 2) > N)
        maxDepth = minDepth + 2;
    else
        maxDepth = N;

    stretchDepth = maxDepth + 1;

    stretchTreeAlloc = NewTreeNodeBufferedAllocator(stretchDepth);
    stretchTree = BottomUpTreeWithAllocator(stretchDepth, stretchTreeAlloc);
    printf(
        "stretch tree of depth %u\t check: %li\n",
        stretchDepth,
        ItemCheck(stretchTree));

    DeleteTreenodeBufferedAllocator(stretchTreeAlloc);

    longLivedTreeAlloc = NewTreeNodeBufferedAllocator(maxDepth);
    longLivedTree = BottomUpTreeWithAllocator(maxDepth, longLivedTreeAlloc);

    for (depth = minDepth; depth <= maxDepth; depth += 2)
    {
        long i, iterations, check;

        iterations = pow(2, maxDepth - depth + minDepth);

        check = 0;
        for (i = 1; i <= iterations; i++)
        {
            treeAlloc = NewTreeNodeBufferedAllocator(depth);
            tempTree = BottomUpTreeWithAllocator(depth, treeAlloc);
            check += ItemCheck(tempTree);
            DeleteTreenodeBufferedAllocator(treeAlloc);
        } /* for(i = 1...) */

        printf(
            "%li\t trees of depth %u\t check: %li\n",
            iterations,
            depth,
            check);
    } /* for(depth = minDepth...) */

    printf(
        "long lived tree of depth %u\t check: %li\n",
        maxDepth,
        ItemCheck(longLivedTree));

    DeleteTreenodeBufferedAllocator(longLivedTreeAlloc);
    
    return 0;
} /* main() */