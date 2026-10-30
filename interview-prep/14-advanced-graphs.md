# Advanced Graphs: Dijkstra, Union-Find, MST: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Graph traversal and shortest paths](../docs/graph-paths/README.md), [Connectivity, MST, and 2-SAT](../docs/connectivity/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 14 ⭐ problems first.

**Recognise it when:** weighted edges, "minimum cost/time/effort path", dynamic connectivity ("after each operation, how many components?"), or "connect everything as cheaply as possible".

1. **Dijkstra:** `(dist, node)` min-heap; skip stale entries (`d > dist[u]`). Non-negative weights only.
2. **Minimax paths** (minimum effort, rising water): Dijkstra with `max` instead of `+`, or sort edges + DSU, or binary search + BFS. Know all three.
3. **0-1 BFS:** deque, push weight-0 edges to the front.
4. **Union-Find** with path compression + union by size: near-O(1). Count components by decrementing on every successful union.
5. **Offline + DSU:** sort queries and edges by threshold, add edges progressively (Grid Queries, Edge Length Limited Paths).
6. **MST:** Kruskal (sort + DSU) for sparse graphs, Prim for dense (all-pairs points).
7. Bellman–Ford with a stops limit (Cheapest Flights, 787) and Dijkstra-with-counting (1976) are in the [DP list](15-dynamic-programming.md).

---

## 1. Dijkstra

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 743 | [Network Delay Time](https://leetcode.com/problems/network-delay-time/) ⭐ | Medium | standard Dijkstra | `G` 🔥 |
| ☐ | 1514 | [Path with Maximum Probability](https://leetcode.com/problems/path-with-maximum-probability/) | Medium | max-heap on probability | `G` |
| ☐ | 505 | [The Maze II](https://leetcode.com/problems/the-maze-ii/) 🔒 | Medium | Dijkstra where an edge = a roll | `G` `U` |
| ☐ | 3341 | [Find Minimum Time to Reach Last Room I](https://leetcode.com/problems/find-minimum-time-to-reach-last-room-i/) ⭐ | Medium | Dijkstra with time-gated cells | `G` `U` |
| ☐ | 3342 | [Find Minimum Time to Reach Last Room II](https://leetcode.com/problems/find-minimum-time-to-reach-last-room-ii/) | Medium | add alternating move cost to the state | `G` `U` |
| ☐ | 1334 | [Find the City With the Smallest Number of Neighbors at a Threshold Distance](https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/) | Medium | Floyd–Warshall or n × Dijkstra | `G` `U` |
| ☐ | 2203 | [Minimum Weighted Subgraph With the Required Paths](https://leetcode.com/problems/minimum-weighted-subgraph-with-the-required-paths/) | Hard | three Dijkstras, pick meeting node | `G` |

## 2. Minimax Paths & 0-1 BFS

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1631 | [Path With Minimum Effort](https://leetcode.com/problems/path-with-minimum-effort/) ⭐ | Medium | Dijkstra on max edge | `G` 🔥 |
| ☐ | 778 | [Swim in Rising Water](https://leetcode.com/problems/swim-in-rising-water/) ⭐ | Hard | Dijkstra / DSU / binary search | `G` 🔥 |
| ☐ | 2812 | [Find the Safest Path in a Grid](https://leetcode.com/problems/find-the-safest-path-in-a-grid/) ⭐ | Medium | multi-source BFS + max-min path | `G` `U` 🔥 |
| ☐ | 1368 | [Minimum Cost to Make at Least One Valid Path in a Grid](https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/) ⭐ | Hard | 0-1 BFS on arrow changes | `G` `U` |
| ☐ | 2290 | [Minimum Obstacle Removal to Reach Corner](https://leetcode.com/problems/minimum-obstacle-removal-to-reach-corner/) | Hard | 0-1 BFS on obstacles | `G` |

## 3. Union-Find

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 684 | [Redundant Connection](https://leetcode.com/problems/redundant-connection/) ⭐ | Medium | first edge joining a component | `G` |
| ☐ | 721 | [Accounts Merge](https://leetcode.com/problems/accounts-merge/) ⭐ | Medium | union emails, group by root | `G` `U` |
| ☐ | 305 | [Number of Islands II](https://leetcode.com/problems/number-of-islands-ii/) ⭐ 🔒 | Hard | add land, union neighbours, count | `G` `U` 🔥 |
| ☐ | 1101 | [The Earliest Moment When Everyone Become Friends](https://leetcode.com/problems/the-earliest-moment-when-everyone-become-friends/) ⭐ 🔒 | Medium | sort logs, stop at 1 component | `G` `U` 🔥 |
| ☐ | 990 | [Satisfiability of Equality Equations](https://leetcode.com/problems/satisfiability-of-equality-equations/) | Medium | union `==` first, then check `!=` | `G` |
| ☐ | 1202 | [Smallest String With Swaps](https://leetcode.com/problems/smallest-string-with-swaps/) | Medium | sort characters within each component | `G` |
| ☐ | 2948 | [Make Lexicographically Smallest Array by Swapping Elements](https://leetcode.com/problems/make-lexicographically-smallest-array-by-swapping-elements/) | Medium | group sorted values by limit, refill | `G` `U` 🔥 |
| ☐ | 959 | [Regions Cut By Slashes](https://leetcode.com/problems/regions-cut-by-slashes/) | Medium | split each cell into 4 triangles | `G` `U` |
| ☐ | 947 | [Most Stones Removed with Same Row or Column](https://leetcode.com/problems/most-stones-removed-with-same-row-or-column/) | Medium | union rows and columns | `G` |
| ☐ | 839 | [Similar String Groups](https://leetcode.com/problems/similar-string-groups/) | Hard | union similar strings | `G` |
| ☐ | 1579 | [Remove Max Number of Edges to Keep Graph Fully Traversable](https://leetcode.com/problems/remove-max-number-of-edges-to-keep-graph-fully-traversable/) | Hard | two DSUs, shared edges first | `G` `U` |

## 4. Offline Queries with Union-Find

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 2503 | [Maximum Number of Points From Grid Queries](https://leetcode.com/problems/maximum-number-of-points-from-grid-queries/) ⭐ | Hard | sort queries + cells, union progressively | `G` `U` 🔥 |
| ☐ | 1697 | [Checking Existence of Edge Length Limited Paths](https://leetcode.com/problems/checking-existence-of-edge-length-limited-paths/) | Hard | sort edges and queries by limit | `G` `U` |
| ☐ | 2092 | [Find All People With Secret](https://leetcode.com/problems/find-all-people-with-secret/) | Hard | group meetings by time, reset non-secret | `G` `U` |
| ☐ | 2421 | [Number of Good Paths](https://leetcode.com/problems/number-of-good-paths/) | Hard | sort nodes by value, union upwards | `G` |
| ☐ | 803 | [Bricks Falling When Hit](https://leetcode.com/problems/bricks-falling-when-hit/) | Hard | reverse time: add bricks back | `G` |

## 5. Minimum Spanning Tree

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1584 | [Min Cost to Connect All Points](https://leetcode.com/problems/min-cost-to-connect-all-points/) ⭐ | Medium | Prim O(n²) on dense points | `G` `U` |
| ☐ | 1489 | [Find Critical and Pseudo-Critical Edges in Minimum Spanning Tree](https://leetcode.com/problems/find-critical-and-pseudo-critical-edges-in-minimum-spanning-tree/) | Hard | Kruskal with forced / skipped edge | `G` `U` |

## 6. Bridges & Euler Paths

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1192 | [Critical Connections in a Network](https://leetcode.com/problems/critical-connections-in-a-network/) ⭐ | Hard | Tarjan low-link bridges | `G` |
| ☐ | 332 | [Reconstruct Itinerary](https://leetcode.com/problems/reconstruct-itinerary/) ⭐ | Hard | Hierholzer with sorted adjacency | `G` `U` |
| ☐ | 753 | [Cracking the Safe](https://leetcode.com/problems/cracking-the-safe/) | Hard | Euler circuit on de Bruijn graph | `G` |

---

**Interview-ready when:** you can implement Dijkstra and DSU from memory in under 10 minutes combined, and explain why Dijkstra fails with negative edges.
