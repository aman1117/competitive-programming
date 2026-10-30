# Graphs: BFS, DFS & Topological Sort: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Graph traversal and shortest paths](../docs/graph-paths/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 14 ⭐ problems first.

**Recognise it when:** grids, cities, routes, dependencies, "minimum number of steps/transfers". **This is Uber's most-tagged topic**: routing, buses, and islands dominate its tag.

1. Build an adjacency list first; for grids use a `dirs` array and an in-bounds check.
2. **BFS = shortest path in an unweighted graph.** Mark visited **when pushing**, not when popping.
3. **Multi-source BFS:** push all sources at distance 0 (Rotting Oranges, 01 Matrix, Walls and Gates).
4. **The state may be more than the node:** `(cell, keys mask)`, `(cell, obstacles left)`, `(route)` instead of `(stop)` in Bus Routes.
5. **Topological sort (Kahn):** indegree-0 queue; if fewer than `n` nodes are output, there is a cycle.
6. **DFS colouring** (white/grey/black) for cycle detection in directed graphs; two-colouring for bipartite checks.

---

## 1. Grid DFS / BFS

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 200 | [Number of Islands](https://leetcode.com/problems/number-of-islands/) ⭐ | Medium | flood fill each unvisited land | `G` `U` 🔥 |
| ☐ | 733 | [Flood Fill](https://leetcode.com/problems/flood-fill/) | Easy | warm-up flood fill | `G` `U` |
| ☐ | 695 | [Max Area of Island](https://leetcode.com/problems/max-area-of-island/) | Medium | flood fill returning size | `G` |
| ☐ | 130 | [Surrounded Regions](https://leetcode.com/problems/surrounded-regions/) | Medium | flood from the border first | `G` `U` |
| ☐ | 417 | [Pacific Atlantic Water Flow](https://leetcode.com/problems/pacific-atlantic-water-flow/) ⭐ | Medium | reverse flow from both oceans | `G` |
| ☐ | 1254 | [Number of Closed Islands](https://leetcode.com/problems/number-of-closed-islands/) | Medium | remove border-connected land first | `G` |
| ☐ | 827 | [Making A Large Island](https://leetcode.com/problems/making-a-large-island/) ⭐ | Hard | label islands, try each water cell | `G` `U` 🔥 |

## 2. Multi-source & Shortest-step BFS

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 994 | [Rotting Oranges](https://leetcode.com/problems/rotting-oranges/) ⭐ | Medium | multi-source BFS by minutes | `G` `U` 🔥 |
| ☐ | 542 | [01 Matrix](https://leetcode.com/problems/01-matrix/) | Medium | multi-source BFS from zeros | `G` |
| ☐ | 286 | [Walls and Gates](https://leetcode.com/problems/walls-and-gates/) 🔒 | Medium | multi-source BFS from gates | `G` `U` |
| ☐ | 1091 | [Shortest Path in Binary Matrix](https://leetcode.com/problems/shortest-path-in-binary-matrix/) | Medium | 8-direction BFS | `G` `U` 🔥 |
| ☐ | 934 | [Shortest Bridge](https://leetcode.com/problems/shortest-bridge/) ⭐ | Medium | DFS one island, BFS to the other | `G` `U` |
| ☐ | 490 | [The Maze](https://leetcode.com/problems/the-maze/) 🔒 | Medium | roll until a wall, BFS on stops | `G` `U` |
| ☐ | 1197 | [Minimum Knight Moves](https://leetcode.com/problems/minimum-knight-moves/) 🔒 | Medium | BFS with symmetry pruning | `G` `U` |

## 3. BFS on an Expanded State

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1293 | [Shortest Path in a Grid with Obstacles Elimination](https://leetcode.com/problems/shortest-path-in-a-grid-with-obstacles-elimination/) ⭐ | Hard | state = (r, c, eliminations left) | `G` `U` |
| ☐ | 864 | [Shortest Path to Get All Keys](https://leetcode.com/problems/shortest-path-to-get-all-keys/) | Hard | state = (r, c, keys mask) | `G` `U` |
| ☐ | 773 | [Sliding Puzzle](https://leetcode.com/problems/sliding-puzzle/) | Hard | state = board string | `G` `U` |
| ☐ | 127 | [Word Ladder](https://leetcode.com/problems/word-ladder/) ⭐ | Hard | wildcard buckets / bidirectional BFS | `G` `U` |
| ☐ | 815 | [Bus Routes](https://leetcode.com/problems/bus-routes/) ⭐ | Hard | BFS over routes, not stops | `G` `U` 🔥 |

## 4. General Graph Traversal

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 133 | [Clone Graph](https://leetcode.com/problems/clone-graph/) ⭐ | Medium | hash map old → new | `G` `U` |
| ☐ | 547 | [Number of Provinces](https://leetcode.com/problems/number-of-provinces/) | Medium | DFS or DSU components | `G` `U` 🔥 |
| ☐ | 841 | [Keys and Rooms](https://leetcode.com/problems/keys-and-rooms/) | Medium | DFS reachability | `G` |
| ☐ | 785 | [Is Graph Bipartite?](https://leetcode.com/problems/is-graph-bipartite/) ⭐ | Medium | two-colouring | `G` `U` |
| ☐ | 399 | [Evaluate Division](https://leetcode.com/problems/evaluate-division/) ⭐ | Medium | weighted graph, DFS product | `G` `U` |
| ☐ | 3387 | [Maximize Amount After Two Days of Conversions](https://leetcode.com/problems/maximize-amount-after-two-days-of-conversions/) | Medium | best rate graph per day, combine | `G` `U` |
| ☐ | 2101 | [Detonate the Maximum Bombs](https://leetcode.com/problems/detonate-the-maximum-bombs/) | Medium | directed reachability per bomb | `G` `U` |
| ☐ | 1376 | [Time Needed to Inform All Employees](https://leetcode.com/problems/time-needed-to-inform-all-employees/) | Medium | DFS on manager tree | `G` |

## 5. Topological Sort

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 207 | [Course Schedule](https://leetcode.com/problems/course-schedule/) ⭐ | Medium | Kahn or DFS colouring | `G` `U` 🔥 |
| ☐ | 210 | [Course Schedule II](https://leetcode.com/problems/course-schedule-ii/) ⭐ | Medium | output Kahn order | `G` `U` 🔥 |
| ☐ | 269 | [Alien Dictionary](https://leetcode.com/problems/alien-dictionary/) ⭐ 🔒 | Hard | edges from first differing chars | `G` `U` 🔥 |
| ☐ | 310 | [Minimum Height Trees](https://leetcode.com/problems/minimum-height-trees/) | Medium | peel leaves layer by layer | `G` 🔥 |
| ☐ | 802 | [Find Eventual Safe States](https://leetcode.com/problems/find-eventual-safe-states/) | Medium | reverse graph + Kahn | `G` `U` |
| ☐ | 2115 | [Find All Possible Recipes from Given Supplies](https://leetcode.com/problems/find-all-possible-recipes-from-given-supplies/) | Medium | Kahn over ingredients | `G` `U` |
| ☐ | 1462 | [Course Schedule IV](https://leetcode.com/problems/course-schedule-iv/) | Medium | topo order + reachability sets | `G` `U` |
| ☐ | 1136 | [Parallel Courses](https://leetcode.com/problems/parallel-courses/) 🔒 | Medium | Kahn level count | `G` `U` |
| ☐ | 2050 | [Parallel Courses III](https://leetcode.com/problems/parallel-courses-iii/) | Hard | longest path in DAG via Kahn | `G` |
| ☐ | 444 | [Sequence Reconstruction](https://leetcode.com/problems/sequence-reconstruction/) 🔒 | Medium | unique order ⇔ queue size 1 | `G` |
| ☐ | 2392 | [Build a Matrix With Conditions](https://leetcode.com/problems/build-a-matrix-with-conditions/) | Hard | two independent topo sorts | `G` |
| ☐ | 1203 | [Sort Items by Groups Respecting Dependencies](https://leetcode.com/problems/sort-items-by-groups-respecting-dependencies/) | Hard | two-level topological sort | `G` |

---

**Interview-ready when:** you can model an unfamiliar story (buses, keys, recipes) as nodes + edges + state within 5 minutes and pick BFS vs DFS vs topological sort with a reason.
