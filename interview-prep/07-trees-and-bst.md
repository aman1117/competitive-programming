# Binary Trees & BSTs: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Trees, ancestors, and rerooting](../docs/trees/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 13 ⭐ problems first.

**Recognise it when:** the input is a `TreeNode*` or an edge list with `n − 1` edges.

1. **Decide what each call returns** (height? `{robbed, skipped}`? a path sum?) and what it updates globally. This is the whole problem.
2. **BFS** for anything "level", "view", or "width"; **DFS** for paths and subtree aggregates.
3. **BST = sorted in-order.** Kth smallest, validation, iterator, and recover-BST all fall out of in-order traversal.
4. **Path sum to a target** on a tree = prefix sums + hash map on the root-to-node path (undo on return).
5. Converting a tree into a **graph** (parent pointers) unlocks distance-K and step-by-step directions.
6. Tree DP problems (robber, cameras, max path sum, rerooting) live in the [DP list](15-dynamic-programming.md).

---

## 1. Traversals & Views

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 104 | [Maximum Depth of Binary Tree](https://leetcode.com/problems/maximum-depth-of-binary-tree/) | Easy | warm-up DFS | `G` `U` 🔥 |
| ☐ | 226 | [Invert Binary Tree](https://leetcode.com/problems/invert-binary-tree/) | Easy | swap children | `G` |
| ☐ | 101 | [Symmetric Tree](https://leetcode.com/problems/symmetric-tree/) | Easy | mirror recursion | `G` |
| ☐ | 102 | [Binary Tree Level Order Traversal](https://leetcode.com/problems/binary-tree-level-order-traversal/) ⭐ | Medium | BFS by level size | `G` 🔥 |
| ☐ | 103 | [Binary Tree Zigzag Level Order Traversal](https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/) | Medium | BFS + alternate direction | `G` |
| ☐ | 199 | [Binary Tree Right Side View](https://leetcode.com/problems/binary-tree-right-side-view/) ⭐ | Medium | last node per level | `G` `U` |
| ☐ | 662 | [Maximum Width of Binary Tree](https://leetcode.com/problems/maximum-width-of-binary-tree/) | Medium | index nodes 2i / 2i + 1 | `G` |
| ☐ | 314 | [Binary Tree Vertical Order Traversal](https://leetcode.com/problems/binary-tree-vertical-order-traversal/) 🔒 | Medium | BFS with column index | `G` |
| ☐ | 987 | [Vertical Order Traversal of a Binary Tree](https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree/) ⭐ | Hard | sort by (col, row, val) | `G` `U` |

## 2. Construction & Serialization

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 105 | [Construct Binary Tree from Preorder and Inorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) ⭐ | Medium | preorder root + inorder index map | `G` |
| ☐ | 297 | [Serialize and Deserialize Binary Tree](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) ⭐ | Hard | preorder with null markers | `G` `U` |
| ☐ | 1008 | [Construct Binary Search Tree from Preorder Traversal](https://leetcode.com/problems/construct-binary-search-tree-from-preorder-traversal/) | Medium | bounds recursion or stack | `G` |
| ☐ | 2196 | [Create Binary Tree From Descriptions](https://leetcode.com/problems/create-binary-tree-from-descriptions/) | Medium | map value → node, find root | `G` `U` |
| ☐ | 652 | [Find Duplicate Subtrees](https://leetcode.com/problems/find-duplicate-subtrees/) | Medium | serialize subtrees into a map | `G` `U` |
| ☐ | 427 | [Construct Quad Tree](https://leetcode.com/problems/construct-quad-tree/) ⭐ | Medium | divide & conquer on quadrants | `G` `U` 🔥 |

## 3. Paths & Ancestors

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 236 | [Lowest Common Ancestor of a Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/) ⭐ | Medium | return node if found in subtree | `G` |
| ☐ | 235 | [Lowest Common Ancestor of a Binary Search Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/) | Medium | use BST ordering | `G` 🔥 |
| ☐ | 1650 | [Lowest Common Ancestor of a Binary Tree III](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree-iii/) 🔒 | Medium | two-pointer on parent chains | `G` |
| ☐ | 113 | [Path Sum II](https://leetcode.com/problems/path-sum-ii/) | Medium | backtrack the path | `G` |
| ☐ | 437 | [Path Sum III](https://leetcode.com/problems/path-sum-iii/) ⭐ | Medium | prefix sum on root-to-node path | `G` |
| ☐ | 863 | [All Nodes Distance K in Binary Tree](https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/) ⭐ | Medium | parent map + BFS | `G` |
| ☐ | 2096 | [Step-By-Step Directions From a Binary Tree Node to Another](https://leetcode.com/problems/step-by-step-directions-from-a-binary-tree-node-to-another/) | Medium | LCA + two paths | `G` |
| ☐ | 1110 | [Delete Nodes And Return Forest](https://leetcode.com/problems/delete-nodes-and-return-forest/) | Medium | post-order, detach deleted | `G` |

## 4. BST

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 98 | [Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/) ⭐ | Medium | pass (low, high) bounds | `G` 🔥 |
| ☐ | 230 | [Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst/) ⭐ | Medium | in-order, stop at k | `G` `U` 🔥 |
| ☐ | 450 | [Delete Node in a BST](https://leetcode.com/problems/delete-node-in-a-bst/) ⭐ | Medium | replace with successor | `G` `U` |
| ☐ | 173 | [Binary Search Tree Iterator](https://leetcode.com/problems/binary-search-tree-iterator/) ⭐ | Medium | controlled in-order with a stack | `G` |
| ☐ | 99 | [Recover Binary Search Tree](https://leetcode.com/problems/recover-binary-search-tree/) | Medium | find two inversions in in-order | `G` |
| ☐ | 653 | [Two Sum IV - Input is a BST](https://leetcode.com/problems/two-sum-iv-input-is-a-bst/) | Easy | in-order + two pointers | `G` |
| ☐ | 272 | [Closest Binary Search Tree Value II](https://leetcode.com/problems/closest-binary-search-tree-value-ii/) 🔒 | Hard | two stacks (predecessors / successors) | `G` `U` |

## 5. Structure Checks

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 110 | [Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree/) | Easy | return −1 for unbalanced | `G` 🔥 |
| ☐ | 572 | [Subtree of Another Tree](https://leetcode.com/problems/subtree-of-another-tree/) | Easy | same-tree at every node | `G` |
| ☐ | 958 | [Check Completeness of a Binary Tree](https://leetcode.com/problems/check-completeness-of-a-binary-tree/) | Medium | BFS: no node after first null | `G` |
| ☐ | 222 | [Count Complete Tree Nodes](https://leetcode.com/problems/count-complete-tree-nodes/) | Medium | compare left/right heights | `G` |
| ☐ | 114 | [Flatten Binary Tree to Linked List](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/) | Medium | reverse pre-order or Morris | `G` |

## 6. Advanced

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1483 | [Kth Ancestor of a Tree Node](https://leetcode.com/problems/kth-ancestor-of-a-tree-node/) | Hard | binary lifting | `G` |
| ☐ | 2791 | [Count Paths That Can Form a Palindrome in a Tree](https://leetcode.com/problems/count-paths-that-can-form-a-palindrome-in-a-tree/) | Hard | XOR bitmask from root + hash map | `G` `U` |

---

**Interview-ready when:** for any tree question you can say, in one sentence, what the recursive function returns, and you can switch between recursive and iterative traversals.
