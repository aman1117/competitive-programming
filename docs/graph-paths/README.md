# Graph traversal and shortest paths

[Handbook](../README.md) | [Graph source](../../include/cp/graphs.hpp) | [Grid patterns](../../include/cp/patterns.hpp)

**Prerequisites:** adjacency lists, queues, heaps, and relaxation.  
**Goal:** model the correct graph and choose a path algorithm whose assumptions match its edges.

## Selection guide

| Edge/state model | Algorithm |
|---|---|
| Unweighted edges | BFS |
| Weights only zero or one | 0-1 BFS |
| Arbitrary nonnegative weights | Dijkstra |
| Negative weights, single source | Bellman-Ford |
| Small graph, all-pairs distances, no negative cycle | Floyd-Warshall |
| Dependencies without cycles | Topological ordering and DAG DP |
| Position alone does not describe future possibilities | Expand the vertex into a compound state |

`Graph` stores integer neighbors. `WeightedGraph` stores `{to,weight}`. An adjacency list describes directed edges unless both directions are inserted.

## 1. BFS, DFS, and path reconstruction

**BFS invariant:** when a vertex is first discovered, its number of edges from the source is minimal. Every layer is processed before the next.

For edges `0->1`, `0->2`, `1->3`, `2->3`, BFS gives distances `[0,1,1,2]`. Vertex `3` can be reached through `1` or `2`; the predecessor array retains one shortest path.

**DFS invariant:** a frame stays active while its next child is explored. `dfs_order` uses explicit continuation frames, so its preorder matches a recursive traversal in adjacency order without requiring a deep native call stack.

```cpp
cp::Graph g{{1, 2}, {3}, {3}, {}, {}};
auto result = cp::bfs(g, {0});
assert((result.distance == std::vector<int>{0, 1, 1, 2, -1}));
assert((cp::restore_path(result.parent, 0, 3) == std::vector<int>{0, 1, 3}));
assert(cp::restore_path(result.parent, 0, 4).empty());
assert((cp::dfs_order(g, 0) == std::vector<int>{0, 1, 3, 2}));
```

**TC/SC:** single-source BFS and DFS take `O(V+E)` time and `O(V)` workspace, excluding output arrays. `restore_path` takes time proportional to the traversed predecessor chain and `O(1)` workspace excluding its returned path. It checks reachability before allocating, so an unreachable query does not secretly allocate a discarded path.

DFS does not generally find shortest paths. Both traversals visit only vertices reachable from their supplied start(s); loop over unvisited vertices when processing every component.

## 2. Multisource and grid BFS

**Use when:** several positions act as equally valid starting points, such as multiple infection sources.

Initialize every source with distance zero and put them all in the queue. This is equivalent to a virtual source with zero-cost connections, without adding an extra distance step.

```cpp
cp::Graph line{{1}, {0, 2}, {1, 3}, {2}};
assert((cp::bfs(line, {0, 3}).distance == std::vector<int>{0, 1, 1, 0}));
auto grid = cp::grid_bfs({"..#", ".#.", "..."}, {{0, 0}});
assert(grid[1][2] == 5);
assert(grid[0][2] == -1); // Blocked.
```

The grid path to `(1,2)` goes down the left column, across the bottom row, and up once. `grid_bfs` uses four-direction movement and `#` as the only blocked character.

**TC/SC:** graph BFS costs `O(V+E+S)` for `S` supplied sources, including duplicate entries. Grid BFS costs `O(R(C+1)+S)` time and `O(RC)` queue space, plus the distance matrix. Different movement rules, edge costs, or cell semantics require changing the model.

## 3. Topological ordering and increasing matrix paths

**Use when:** every transition follows a dependency that must be processed first.

Kahn's algorithm repeatedly removes vertices with indegree zero. Removing an outgoing edge may make another vertex ready. If fewer than `V` vertices are removed, a cycle exists.

```cpp
auto order = cp::topological_sort({{1, 2}, {3}, {3}, {}});
assert(order && *order == std::vector<int>({0, 1, 2, 3}));
assert(!cp::topological_sort({{1}, {0}}));
assert(cp::longest_increasing_path({{9, 9, 4}, {6, 6, 8}, {2, 1, 1}}) == 4);
```

For increasing matrix paths, orient every neighboring edge from smaller to larger. Strict increase makes a cycle impossible. Removing indegree-zero layers counts longest-path length: the example contains `1 -> 2 -> 6 -> 9`.

**TC/SC:** topological sort takes `O(V+E)` time and `O(V)` space. The matrix routine takes `O(R(C+1))` time and space, normally `O(RC)`. This is an iterative alternative to memoized DFS whose native recursion could reach `R*C`.

## 4. Dijkstra

**Use when:** all reachable edge weights are nonnegative.

Extract the smallest tentative distance from a min-heap. No unprocessed route can later improve that vertex: reaching it through a vertex with at least as large a distance and a nonnegative extra edge cannot produce a smaller distance.

Consider `0->1(4)`, `0->2(1)`, `2->1(2)`, `1->3(1)`, and `2->3(5)`.

| Settled vertex | Important improvement |
|---|---|
| 0 at distance 0 | Tentative `d[1]=4`, `d[2]=1` |
| 2 at distance 1 | Improve `d[1]=3`, set `d[3]=6` |
| 1 at distance 3 | Improve `d[3]=4` |
| 3 at distance 4 | Final shortest distance |

```cpp
cp::WeightedGraph g(4);
g[0] = {{1, 4}, {2, 1}};
g[1] = {{3, 1}};
g[2] = {{1, 2}, {3, 5}};
auto result = cp::dijkstra(g, 0);
assert((result.distance == std::vector<cp::i64>{0, 3, 1, 4}));
assert((cp::restore_path(result.parent, 0, 3) == std::vector<int>{0, 2, 1, 3}));
```

The heap still contains stale entries such as `(4,1)` and `(6,3)`. They must be skipped when their distance differs from the current best.

**TC/SC:** the lazy heap version takes `O(V+E log(E+1))` time and `O(V+E)` space. Heap storage is not necessarily `O(V)`. Negative weights invalidate the settling proof; use an appropriate alternative instead.

## 5. 0-1 BFS

**Use when:** every edge costs exactly zero or one.

A zero-cost relaxation goes to the deque's front; a one-cost relaxation goes to its back. This maintains the next useful distance order without a logarithmic heap.

```cpp
cp::WeightedGraph g(4);
g[0] = {{1, 1}, {2, 0}};
g[1] = {{3, 0}};
g[2] = {{1, 0}, {3, 1}};
assert(cp::zero_one_bfs(g, 0)[3] == 0);
```

The zero-cost route is `0->2->1->3`, even though it uses more edges than `0->1->3`.

**TC/SC:** `O(V+E)` time and `O(V+E)` space for this lazy-entry deque implementation. Ordinary BFS minimizes the number of edges, not the weighted cost. Weights outside `{0,1}` require a different queue discipline.

## 6. Bellman-Ford and negative cycles

**Use when:** negative edges are allowed and single-source shortest distances or negative-cycle effects are needed.

Without a reachable negative cycle, a shortest simple path uses at most `V-1` edges. Repeatedly relaxing all edges for up to `V-1` passes is therefore sufficient. Any further improvement indicates influence from a reachable negative cycle.

```cpp
auto finite = cp::bellman_ford(4, {{0, 1, 5}, {0, 2, 2}, {1, 2, -7}, {2, 3, 1}}, 0);
assert((finite.distance == std::vector<cp::i64>{0, 5, -2, -1}));
auto cycle = cp::bellman_ford(5, {{0, 1, 1}, {1, 2, -2}, {2, 1, 1}, {2, 3, 4}}, 0);
assert(cycle.negative_infinite[1] && cycle.negative_infinite[2] && cycle.negative_infinite[3]);
assert(!cycle.negative_infinite[4] && cycle.distance[4] == cp::INF);
```

The cycle `1->2->1` has weight `-1`. Repeating it makes costs arbitrarily small; vertex `3` is also affected because the cycle can reach it. Vertex `4` is unreachable and is not marked.

**TC/SC:** `O(VE+V+E)` time, `O(V+E)` space, including propagation of affected vertices. A `negative_infinite` flag overrides the provisional numeric distance. Repeated negative relaxations can overflow even when ordinary simple-path sums would fit; obey the stronger arithmetic bound in the source.

## 7. Floyd-Warshall

**Use when:** all-pairs distances are needed on a small graph.

After processing intermediate vertex `k`, `d[i][j]` is the best route whose intermediate vertices come from the processed set. Either that route avoids `k`, or it splits into `i->k` and `k->j`.

```cpp
cp::Matrix distances{{0, 3, cp::INF}, {cp::INF, 0, -1}, {4, cp::INF, 0}};
auto result = cp::floyd_warshall(distances);
assert(result[0][2] == 2); // 0->1->2.
assert(result[1][0] == 3); // 1->2->0.
```

**TC/SC:** three vertex loops give `O(V^3)` time; the by-value working/result matrix uses `O(V^2)` space. Initialize diagonal entries to zero, absent edges to `INF`, and parallel edges to their minimum weight. Do not add `INF` to another distance. This wrapper requires no negative cycles; it does not return pairwise negative-infinity information.

## 8. BFS over state masks

**Use when:** arriving at the same vertex with different history changes what remains to be achieved.

To visit every vertex in a shortest walk, use `(vertex,visited_mask)`. A single `visited[vertex]` would wrongly discard useful revisits.

For the four-vertex star, `1->0->2->0->3` visits all vertices in four edges. Returning to `0` with leaves already visited is different from starting at `0`.

```cpp
assert(cp::shortest_visit_all({{1, 2, 3}, {0}, {0}, {0}}) == 4);
assert(cp::shortest_visit_all({{1}, {2}, {}}) == 2);
assert(cp::shortest_visit_all({{}, {}}) == -1);
```

**TC/SC:** at most `V*2^V` states and `E*2^V` outgoing transitions: `O((V+E)*2^V)` time, `O(V*2^V)` space. The implementation is intended for roughly `V<=15`; its hard limit of 20 does not imply low memory usage.

This is not Hamiltonian-cycle DP: revisits are allowed, any vertex may be the start, and returning to the start is unnecessary. For keys, fuel, coupons, or alternating edge colors, derive the additional state from what determines future legal moves.
