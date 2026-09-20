# Trees, ancestors, and rerooting

[Handbook](../README.md) | [Indexed trees](../../include/cp/trees.hpp) | [Binary-tree nodes](../../include/cp/nodes.hpp) | [Functional jumps](../../include/cp/offline.hpp)

**Prerequisites:** [DFS/BFS](../graph-paths/README.md), [range queries](../range-queries/README.md), and the idea of a subtree state.  
**Goal:** distinguish one-query traversals, reusable preprocessing, and all-root dynamic programming.

## 1. Binary lifting for ancestors and LCA

**Use when:** many ancestor or lowest-common-ancestor queries target the same tree.

Precompute `up[b][v]`, the ancestor `2^b` steps above `v`. The recurrence is `up[b][v]=up[b-1][up[b-1][v]]`: two half-jumps make one full jump.

For kth ancestors, decompose `k` into powers of two. For LCA, move one vertex upward using the largest jump that still leaves it outside the other vertex's ancestor chain, then take its parent.

```text
       0
      / \
     1   2
    / \
   3   4
```

```cpp
cp::Graph tree{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
cp::BinaryLifting lift(tree);
assert(lift.lca(3, 4) == 1);
assert(lift.distance(3, 2) == 3);
assert(lift.kth_ancestor(3, 2) == 0);
assert(lift.kth_ancestor(3, 3) == -1);
```

**TC/SC:** `O(n log n)` preprocessing time and storage, `O(log n)` per LCA/distance/kth query, `O(1)` query workspace. Distance is `depth[a]+depth[b]-2*depth[lca]` for an unweighted tree. Input must be a connected, nonempty, undirected tree; cyclic graphs do not satisfy the contract.

## 2. Euler/preorder flattening

**Use when:** subtree operations should become array-range operations.

A DFS visits every subtree contiguously. The constructor exposes `tin[v]`, exclusive `tout[v]`, and `order`; the subtree is exactly `[tin[v],tout[v])`.

For the tree above, this implementation's stack may visit `0,2,1,4,3`. The subtree of `1` still occupies one contiguous interval. Do not assume a particular sibling order; use the returned indices.

```cpp
cp::Graph tree{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
cp::BinaryLifting lift(tree);
std::vector<cp::i64> weight{10, 20, 30, 40, 50}, flat(5);
for (int v = 0; v < 5; ++v) flat[lift.tin[v]] = weight[v];
cp::Fenwick bit(flat);
assert(bit.sum(lift.tin[1], lift.tout[1]) == 110);
assert(lift.is_ancestor(1, 4));
```

**TC/SC:** the timestamps themselves are linear to construct, but this combined class also builds binary lifting, so total preprocessing is `O(n log n)`. Ancestor tests are `O(1)`; Fenwick subtree sums/point changes are `O(log n)`. Do not charge the entire class only the cost of its Euler-tour portion.

## 3. Functional-graph jumping

**Use when:** every vertex has exactly one successor, possibly creating cycles, and queries ask where you end after many steps.

The same doubling recurrence works without a tree or root. A cycle `0->1->2->0` puts vertex zero at vertex one after ten steps.

```cpp
cp::FunctionalJump jump({1, 2, 0}, 1'000'000'000'000'000'000ULL);
assert(jump.jump(0, 10) == 1);
assert(jump.jump(2, 0) == 2);
```

**TC/SC:** `O(nB)` preprocessing/storage and `O(B)` time per query, where `B` is the bit width of the configured maximum step count, at least one. This is not necessarily `log n`: the number of steps may greatly exceed the vertex count. Queries must not exceed the precomputed maximum.

## 4. Tree diameter

**Use when:** finding the longest path, measured in edges, in an unweighted tree.

BFS from any vertex finds a farthest endpoint of some diameter. BFS again from that endpoint finds the other endpoint and the diameter length. The unique-path property of a tree is essential to the two-sweep argument.

```cpp
cp::Graph tree{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
auto diameter = cp::tree_diameter(tree);
assert(diameter.length == 3);
assert(diameter.path.size() == 4);
```

One longest path is `3->1->0->2`; another starts at `4`.

**TC/SC:** `O(n)` time and space. The same two-BFS procedure is not a general longest-path algorithm for arbitrary graphs.

## 5. Rerooting distance sums

**Use when:** every vertex must be considered as a possible root.

First compute subtree sizes and the sum of distances from an arbitrary root. Moving the root across edge `p->v` makes `size[v]` vertices one step closer and the other `n-size[v]` vertices one step farther:

```text
answer[v] = answer[p] - size[v] + (n-size[v])
          = answer[p] + n - 2*size[v]
```

For the example tree, root zero has total distance six. Subtree one has size three, so its answer is `6+5-2*3=5`.

```cpp
cp::Graph tree{{1, 2}, {0, 3, 4}, {0}, {1}, {1}};
assert((cp::tree_distance_sums(tree) == std::vector<cp::i64>{6, 5, 9, 8, 8}));
```

**TC/SC:** `O(n)` time and space. A separate BFS from every vertex would cost `O(n^2)`. This template is specifically for unweighted distance sums; derive a new transition for a different rerooted quantity.

## 6. Take/skip tree DP

**Use when:** choosing a vertex prevents choosing its adjacent vertices.

Let `take[v]` be the best subtree sum when taking `v`, and `skip[v]` when skipping it:

```text
take[v] = weight[v] + sum(skip[child])
skip[v] = sum(max(take[child], skip[child]))
```

For a path with weights `[5,100,6]`, choosing the middle yields `100`, while choosing both endpoints yields `11`.

```cpp
assert(cp::tree_independent_set({{1}, {0, 2}, {1}}, {5, 100, 6}) == 100);
```

**TC/SC:** `O(n)` time and space. Children become independent only after fixing the parent's choice. Empty selection is allowed, so all-negative weights yield zero.

## 7. Generic iterative binary-tree postorder

**Use when:** a binary-tree answer combines results from its left and right subtrees.

`binary_tree_fold(root,empty,combine)` makes the dependency explicit. A frame processes its left child, then right child, then calls `combine(node,left_state,right_state)`. The result returns to the parent through an explicit stack.

```cpp
cp::TreeNode root(2), left(1), right(3);
root.left = &left; root.right = &right;
auto sum = cp::binary_tree_fold(&root, 0LL,
    [](cp::TreeNode* node, cp::i64 l, cp::i64 r) { return node->val + l + r; });
assert(sum == 6);
assert((cp::inorder(&root) == std::vector<int>{1, 2, 3}));
assert((cp::level_order(&root) == std::vector<std::vector<int>>{{2}, {1, 3}}));
assert(cp::is_bst(&root));
right.val = 2;
assert(!cp::is_bst(&root)); // Strict BST: duplicates are forbidden.
```

**TC/SC:** with constant-size state and constant-time combination, the fold takes `O(n)` time and `O(h)` space. Inorder/BST traversal also uses `O(h)` stack space; level-order BFS uses `O(w)` queue space. Returned traversal output adds `O(n)` storage. A chain has `h=n`, not `log n`.

Inorder visits left subtree, node, then right subtree. A strict BST's inorder sequence must be strictly increasing, which is the invariant checked by `is_bst`. `level_order` instead processes a queue one level at a time; neither traversal order is interchangeable with postorder when a parent needs completed child states.

## 8. Height, balance, diameter, and maximum path sum

These can share one postorder traversal, but they need different interpretations of a child result.

Height is one plus the larger child height. Balance requires both subtrees to be balanced and their heights to differ by at most one. A diameter through a node has `left_height+right_height` edges.

For maximum path sum, an upward-extending path can use **one** child branch, while a complete path through the node may use both. Negative child gains are discarded, but a complete answer must remain nonempty.

```text
       -10
       / \
      9  20
         / \
        15  7
```

```cpp
cp::TreeNode root(-10), left(9), right(20), a(15), b(7);
root.left = &left; root.right = &right;
right.left = &a; right.right = &b;
auto stats = cp::binary_tree_stats(&root);
assert(stats.height == 3 && stats.balanced && stats.diameter == 3);
assert(stats.max_path_sum == 42); // 15 + 20 + 7.
```

The upward gain from `20` is `20+15=35`; returning `42` to its parent would incorrectly create a branching path.

**TC/SC:** `O(n)` time, `O(h)` space. Height counts nodes, diameter counts edges. The maximum path is `nullopt` for an empty tree and remains negative when all nonempty choices are negative.

## 9. One-query pointer LCA

**Use when:** only one or a few LCA queries are needed on a pointer-based binary tree.

The fold returns a two-bit mask indicating whether each target was found. The lowest completed subtree containing both targets is their LCA. Pointer identity distinguishes nodes even when values are equal.

```cpp
cp::TreeNode root(1), left(2), right(2), absent(2);
root.left = &left; root.right = &right;
assert(cp::binary_tree_lca(&root, &left, &right) == &root);
assert(cp::binary_tree_lca(&root, &left, &left) == &left);
assert(cp::binary_tree_lca(&root, &left, &absent) == nullptr);
```

**TC/SC:** `O(n)` time and `O(h)` space per query, with no preprocessing. Use indexed binary lifting for many repeated queries. Unlike a common simplified recursive solution, this API explicitly verifies that both nodes exist.

## 10. Prefix sums on the active root-to-node path

**Use when:** counting downward paths with a target sum, allowing arbitrary ancestor/descendant endpoints.

As with array prefix sums, a current prefix `s` pairs with earlier prefixes `s-target`. However, only prefixes on the **current ancestor chain** are eligible.

The traversal pushes enter and exit events. Entering adds a prefix; exiting removes it. Otherwise prefixes from one sibling branch contaminate another.

```cpp
cp::TreeNode root(1), left(-1), right(-1);
root.left = &left; root.right = &right;
assert(cp::count_tree_path_sum(&root, 0) == 2);
```

The two paths are root-to-left and root-to-right. After processing the left branch, its zero prefix must be removed before entering the right branch.

**TC/SC:** `O(n log(h+2))` time and `O(h)` space with the deterministic ordered map used here. An unordered-map variant could be expected linear but would require an explicit expected-time claim.

All node helpers are non-owning. They neither allocate nor delete tree nodes; on LeetCode use the judge's node definitions and copy the required algorithms/dependencies.
