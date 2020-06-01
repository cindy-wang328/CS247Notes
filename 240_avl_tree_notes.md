# CS240-notes
online classes :'( 

AVL Property. At every node, the |height(n.left) - height(n.right)| <= 1
- This means insert, delete, and search are always O(log n) even in worst case
- Let N(h) be the smallest number of nodes for a given height h
- N(h) = 1+N(h-1)+N(h-2) assuming the worst case which is one child is height h-1, the other is height h-2 at every node
  - this is the tallest tree given a number of nodes / the smallest #nodes for h
- N(h) >= 2N(h-2, N(h) >= 2^2 N(h-4)...
  - if h is even: N(h) ≥ 2^(h/2) N(0) → log⁡(N(h)) ≥ h/2 → h ≤ 2log⁡ N(h)
  - if h is odd: N(h) ≥ 2^((h-1)/2) N(1)=2^((h-1)/2) * 2 -> log N(h) ≥(h-1)/2+1 → h ≤ 2log⁡ N(h−2)
- in both cases h is <= log N(h)

AVL Insert
- First use "normal" bst insert to place the new node
- From the new node, move up the tree and update heights: height is 1 + max(height of left, height of right)
- At the first node that is unbalanced (call this z), have to do one of 4 types of restructuring
  - Let y = taller child of z, x = taller child of y
  - Case 1: y = z->left, x = y->left => Right rotation on z
  - Case 2: y = z->right, x = y->right => Left rotation on z
  - Case 3: y = z->left, x = y->right => Left rotation on y, then right rotation on z
  - case 4: y = z->right, x = y->left => Right rotation on y, then left rotation on z
Right rotate is `y = z.left, z.left = y.right, y.right = z`
Example: Adding the 28 will make the tree unbalanced so there was a left rotation on 22, making 28 the new root


```
Add 22
\-- 22 (0) (N)
Add 31
\-- 22 (1) (N)
    \-- 31 (0) (22)
Add 28
\-- 28 (1) (N)
    |-- 22 (0) (28)
    \-- 31 (0) (28)
```

AVL Delete
- Use normal bst delete, and find the parent of the deleted node. Let this be z
- Move up the tree and rebalance with the same 4 cases of rotates if necessary
- Difference with insert: 
  - When inserting, only need to rebalance once. With delete, might have to rebalance multiple times, including rebalance the root
  - tallerChild of y, to determine the rotate case: if there is a tie, return (y.left if y itself is a left child, y.right if y is a right child)
  
```
\-- 22 (4) (N)
    |-- 10 (3) (22)
    |   |-- 6 (1) (10)
    |   |   |-- 4 (0) (6)
    |   |   \-- 8 (0) (6)
    |   \-- 14 (2) (10)
    |       |-- 13 (0) (14)
    |       \-- 18 (1) (14)
    |           |-- 16 (0) (18)
    \-- 31 (2) (22)
        |-- 28 (0) (31)
        \-- 37 (1) (31)
            \-- 46 (0) (37)
            
After deleting 22, it places 28 as the root, and first rebalances 31, 37, and 46
then rebalances 28, 10, 14

\-- 14 (3) (N)
    |-- 10 (2) (14)
    |   |-- 6 (1) (10)
    |   |   |-- 4 (0) (6)
    |   |   \-- 8 (0) (6)
    |   \-- 13 (0) (10)
    \-- 28 (2) (14)
        |-- 18 (1) (28)
        |   |-- 16 (0) (18)
        \-- 37 (1) (28)
            |-- 31 (0) (37)
            \-- 46 (0) (37)
```
