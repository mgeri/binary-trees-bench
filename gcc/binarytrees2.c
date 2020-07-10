#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>

typedef struct tn
{
    struct tn *left;
    struct tn *right;
} treeNode;


static treeNode *NewTreeNode(treeNode *left, treeNode *right)
{
   treeNode *ret;

   ret = malloc(sizeof(treeNode));
   ret->left = left;
   ret->right = right;

   return ret;
}

static long ItemCheck(treeNode *tree)
{
   if (tree->left == NULL)
      return 1;
   else
      return 1 + ItemCheck(tree->left) +
             ItemCheck(tree->right);
}

static treeNode *BottomUpTree(int depth)
{
   if (depth > 0)
      return NewTreeNode(BottomUpTree(depth - 1),
                      BottomUpTree(depth - 1));
   else
      return NewTreeNode(NULL, NULL);
}

static void DeleteTree(treeNode *tree)
{
   if (tree->left != NULL)
   {
      DeleteTree(tree->left);
      DeleteTree(tree->right);
   }
   free(tree);
}

struct workerArgs
{
   long iter, check;
   int depth;
   pthread_t id;
   struct workerArgs *next;
};

static void *checkTreeOfDepth(void *_args)
{
   struct workerArgs *args = _args;
   long i, iter, check, depth;
   treeNode *tmp;

    printf("Available processors %d\n", get_nprocs_conf());

   iter = args->iter;
   depth = args->depth;

   check = 0;
   for (i = 1; i <= iter; i++)
   {
      tmp = BottomUpTree(depth);
      check += ItemCheck(tmp);
      DeleteTree(tmp);
   }

   args->check = check;
   return NULL;
}

int main(int ac, char **av)
{
   treeNode *stretch, *longlived;
   struct workerArgs *args, *targs, *hargs;
   int n, depth, mindepth, maxdepth, stretchdepth;

   printf("Available processors %d\n", get_nprocs_conf());

   n = ac > 1 ? atoi(av[1]) : 10;
   if (n < 1)
   {
      fprintf(stderr, "Wrong argument.\n");
      exit(1);
   }

   mindepth = 4;
   maxdepth = mindepth + 2 > n ? mindepth + 2 : n;
   stretchdepth = maxdepth + 1;

   stretch = BottomUpTree(stretchdepth);
   printf("stretch tree of depth %u\t check: %li\n", stretchdepth,
          ItemCheck(stretch));
   DeleteTree(stretch);

   longlived = BottomUpTree(maxdepth);

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
      printf("%ld\t trees of depth %d\t check: %ld\n",
             args->iter, args->depth, args->check);
      hargs = args->next;
      free(args);
   }

   printf("long lived tree of depth %d\t check: %ld\n", maxdepth,
          ItemCheck(longlived));

   /* not in original C version: */
   DeleteTree(longlived);

   return 0;
}