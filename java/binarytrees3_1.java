
public class binarytrees3_1 {

   private final static int minDepth = 4;

   public static void main(String[] args) {
      
      System.out.println("Available processors " + Runtime.getRuntime().availableProcessors());

      int n = 0;
      if (args.length > 0)
         n = Integer.parseInt(args[0]);

      int maxDepth = (minDepth + 2 > n) ? minDepth + 2 : n;
      int stretchDepth = maxDepth + 1;

      TreeNodeBufferedAllocator stretchAllocator = new TreeNodeBufferedAllocator(stretchDepth);

      int check = (TreeNode.bottomUpTree(stretchDepth, stretchAllocator)).itemCheck();
      System.out.println("stretch tree of depth " + stretchDepth + "\t check: " + check);

      TreeNode longLivedTree = TreeNode.bottomUpTree(maxDepth, new TreeNodeBufferedAllocator(maxDepth));

      for (int depth = minDepth; depth <= maxDepth; depth += 2) {
         int iterations = 1 << (maxDepth - depth + minDepth);
         check = 0;

         for (int i = 1; i <= iterations; i++) {
            stretchAllocator.reset();
            check += (TreeNode.bottomUpTree(depth, stretchAllocator)).itemCheck();
         }
         System.out.println(iterations + "\t trees of depth " + depth + "\t check: " + check);
      }
      System.out.println("long lived tree of depth " + maxDepth + "\t check: " + longLivedTree.itemCheck());
   }

   private static class TreeNode {
      private TreeNode left, right;

      private static TreeNode bottomUpTree(int depth, TreeNodeBufferedAllocator allocator) {
         if (depth > 0) {
            return allocator.newTreeNode(bottomUpTree(depth - 1, allocator), bottomUpTree(depth - 1, allocator));
         } else {
            return allocator.newTreeNode(null, null);
         }
      }

      TreeNode(TreeNode left, TreeNode right) {
         this.left = left;
         this.right = right;
      }

      private int itemCheck() {
         // if necessary deallocate here
         if (left == null)
            return 1;
         else
            return 1 + left.itemCheck() + right.itemCheck();
      }
   }

   // tree node allocator
   private static class TreeNodeBufferedAllocator {
      private TreeNode buffer[];
      private int index = 0;

      TreeNodeBufferedAllocator(int depth) {
         buffer = new TreeNode[1 << (depth + 1)];
         for (int i = 0; i < buffer.length; i++) {
            buffer[i] = new TreeNode(null, null);
         }
      }

      TreeNode newTreeNode(TreeNode left, TreeNode right) {
         TreeNode node = buffer[index];
         node.left = left;
         node.right = right;
         index++;
         return node;
      }

      void reset() {
         index = 0;
      }
   }
}