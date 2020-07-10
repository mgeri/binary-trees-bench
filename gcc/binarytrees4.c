#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
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

struct workerArgs
{
    long iter, check;
    int depth;
    pthread_t id;
    treeNodeBufferedAllocator *allocator;
    struct workerArgs *next;
};

static void *checkTreeOfDepth(void *_args)
{
    struct workerArgs *args = _args;
    long i, iter, check, depth;
    treeNode *tmp;

    iter = args->iter;
    depth = args->depth;

    check = 0;

    // reuse allocator while iterating
    treeNodeBufferedAllocator *allocator = NewTreeNodeBufferedAllocator(depth);
    for (i = 1; i <= iter; i++)
    {
        tmp = BottomUpTreeWithAllocator(depth, allocator);
        check += ItemCheck(tmp);
        allocator->index = 0;
    }
    DeleteTreenodeBufferedAllocator(allocator);

    args->check = check;
    return NULL;
}

int main(int ac, char **av)
{
    treeNode *stretch, *longlived;
    treeNodeBufferedAllocator *stretchAllocator, *longlivedAllocator;

    printf("Available processors %d\n", get_nprocs_conf());

    struct workerArgs *args, *targs, *hargs;
    int n, depth, mindepth, maxdepth, stretchdepth;

    n = ac > 1 ? atoi(av[1]) : 10;
    if (n < 1)
    {
        fprintf(stderr, "Wrong argument.\n");
        exit(1);
    }

    mindepth = 4;
    maxdepth = mindepth + 2 > n ? mindepth + 2 : n;
    stretchdepth = maxdepth + 1;

    stretchAllocator = NewTreeNodeBufferedAllocator(stretchdepth);
    stretch = BottomUpTreeWithAllocator(stretchdepth, stretchAllocator);
    printf("stretch tree of depth %u\t check: %li\n", stretchdepth, ItemCheck(stretch));
    DeleteTreenodeBufferedAllocator(stretchAllocator);

    longlivedAllocator = NewTreeNodeBufferedAllocator(maxdepth);
    longlived = BottomUpTreeWithAllocator(maxdepth, longlivedAllocator);

    hargs = NULL;
    targs = NULL;
    for (depth = mindepth; depth <= maxdepth; depth += 2)
    {

        args = malloc(sizeof(struct workerArgs));
        args->iter = 1 << (maxdepth - depth + mindepth);
        args->depth = depth;
        args->next = NULL;
        if (targs == NULL)
        {
            hargs = args;
            targs = args;
        }
        else
        {
            targs->next = args;
            targs = args;
        }
        pthread_create(&args->id, NULL, checkTreeOfDepth, args);
    }

    while (hargs != NULL)
    {
        args = hargs;
        pthread_join(args->id, NULL);
        printf("%ld\t trees of depth %d\t check: %ld\n", args->iter, args->depth, args->check);
        hargs = args->next;
        free(args);
    }

    printf("long lived tree of depth %d\t check: %ld\n", maxdepth, ItemCheck(longlived));

    /* not in original C version: */
    DeleteTreenodeBufferedAllocator(longlivedAllocator);

    return 0;
}