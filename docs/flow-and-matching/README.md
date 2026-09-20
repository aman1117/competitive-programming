# Maximum flow and bipartite matching

[Handbook](../README.md) | [Implementation](../../include/cp/flow.hpp)

**Prerequisites:** graph traversal, capacities, and [bipartite graphs](../connectivity/README.md).  
**Goal:** understand how reversible local choices build globally optimal allocations. These are useful stretch topics, not prerequisites for every contest problem.

## 1. Residual networks

**Problem:** send as much flow as possible from source `s` to sink `t`, respecting edge capacities and conserving flow at intermediate vertices.

If an edge of capacity five carries two units, its forward residual capacity is three. Its reverse residual edge has capacity two: that is permission to undo an earlier choice, not an additional original connection.

Without reverse edges, an initially valid route can permanently block a better combination of routes. Augmenting algorithms need the ability to reroute existing flow.

Keep original capacity, current flow, and residual capacity conceptually separate. `Dinic` stores residual capacities internally and updates both forward and reverse arcs after every augmentation.

## 2. Dinic's algorithm

**Use when:** a problem can be modeled as integral-capacity directed maximum flow.

Dinic repeats two phases:

1. BFS assigns levels using positive residual edges.
2. DFS sends a blocking flow using only edges that advance one level. Per-vertex current-arc pointers avoid rescanning failed outgoing edges.

When the level graph becomes blocked, rebuild levels. The shortest residual source-to-sink path length increases between successful blocking phases, bounding the number of phases.

### Worked network

```text
0 = source, 3 = sink

0 -> 1 : capacity 3
0 -> 2 : capacity 2
1 -> 2 : capacity 1
1 -> 3 : capacity 2
2 -> 3 : capacity 3
```

The first level graph supports two units through `0->1->3` and two through `0->2->3`. A later phase sends one through `0->1->2->3`. Total flow is five, matching all capacity leaving the source.

```cpp
cp::Dinic flow(4);
flow.add_edge(0, 1, 3);
flow.add_edge(0, 2, 2);
flow.add_edge(1, 2, 1);
flow.add_edge(1, 3, 2);
flow.add_edge(2, 3, 3);
assert(flow.max_flow(0, 3) == 5);
assert(flow.max_flow(0, 3) == 0); // No additional flow in this residual network.
```

**TC/SC:** general-case `O(V^2 E)` time and `O(V+E)` retained graph storage, with `O(V)` auxiliary arrays/DFS depth. There are at most `O(V)` level phases; the standard general blocking-flow bound is `O(VE)` per phase. Faster bounds for special graph classes are not universal guarantees.

**Contracts:** source and sink must differ; capacities and total flow must fit the documented bounds; parallel edges are allowed, self-loops are excluded. Recursive DFS can overflow the native stack on very deep level graphs. The object is stateful: use a fresh object when you need a fresh computation.

## 3. Recovering a minimum cut

After maximum flow, mark every vertex reachable from the source through positive residual edges. This reachable set is the source side of a minimum cut. Every original edge crossing from that set to the unreachable side is saturated.

```cpp
cp::Dinic flow(3);
flow.add_edge(0, 1, 2);
flow.add_edge(1, 2, 5);
assert(flow.max_flow(0, 2) == 2);
auto side = flow.min_cut_side(0);
assert(side[0] && !side[1] && !side[2]);
```

The cut separating `0` from `{1,2}` has capacity two, proving that the computed flow cannot be improved.

**TC/SC:** residual reachability costs `O(V+E)` time and `O(V)` space. Call it after exhausting augmenting paths if you want the minimum-cut interpretation. Keep your original edge list if you need to enumerate cut edges or calculate their original capacities.

## 4. Kuhn's bipartite matching

**Use when:** left-side objects must be paired with distinct compatible right-side objects.

An augmenting path alternates between unmatched and matched edges. Starting from an unmatched left vertex, either find a free right vertex or recursively move its current owner to another right vertex. Flipping that path increases matching size by one.

Example:

```text
L0 -> R0, R1
L1 -> R0
L2 -> R1, R2
```

If `L0` initially takes `R0`, searching for `L1` can move `L0` to `R1`, freeing `R0`. Finally `L2` can use `R2`.

```cpp
auto right_match = cp::bipartite_matching({{0, 1}, {0}, {1, 2}}, 3);
assert((right_match == std::vector<int>{1, 0, 2}));
```

The result maps **right index to left index**, with `-1` for unmatched right vertices.

**TC/SC:** the documented conservative bound is `O(L*(L+E)+R)` time and `O(L+R)` space, including recursion/output. A search explores alternating paths for each left vertex. Putting the smaller partition on the left is often helpful. This is Kuhn's algorithm, not the faster Hopcroft-Karp implementation.

## Modeling checklist

Use unit capacities from source to left vertices, left-to-right compatibility edges, and right vertices to sink to express bipartite matching as flow. Larger capacities express quotas. Do not add costs to a capacity-only network and expect a minimum-cost solution; min-cost flow is a separate algorithm and is not implemented here.
