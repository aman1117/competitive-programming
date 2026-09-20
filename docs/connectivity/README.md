# Connectivity, spanning trees, and 2-SAT

[Handbook](../README.md) | [Connectivity source](../../include/cp/connectivity.hpp) | [Graph source](../../include/cp/graphs.hpp) | [DSU source](../../include/cp/data_structures.hpp)

**Prerequisites:** [BFS/DFS and directed graphs](../graph-paths/README.md).  
**Goal:** reduce global connectivity or logical constraints to components and structural invariants.

## 1. Disjoint-set union

**Use when:** components are merged over time, and you need to know whether two vertices already belong to the same component.

Each component is represented by a parent-pointer tree. Union by size attaches the smaller root under the larger. Path compression/halving shortens future find operations while preserving the representative.

Starting with five isolated vertices, unions `(0,1)` and `(1,2)` create `{0,1,2}`, `{3}`, `{4}`. A later union `(0,2)` reports no change.

```cpp
cp::DSU dsu(5);
assert(dsu.unite(0, 1));
assert(dsu.unite(1, 2));
assert(!dsu.unite(0, 2));
assert(dsu.size(2) == 3 && dsu.components() == 3);
```

**TC/SC:** `O(n)` initialization/storage, amortized `O(alpha(n))` find/union with both optimizations. This is not a literal worst-case constant-time guarantee. Standard DSU does not support deleting an edge or splitting a component.

## 2. Kruskal's minimum spanning tree

**Use when:** connecting every vertex of an undirected graph at minimum total edge cost.

Sort edges by weight and accept an edge exactly when it joins different DSU components. The accepted edges cannot form a cycle. The cut property justifies taking the cheapest edge joining two currently separated groups.

For edges `(0,1,1)`, `(1,2,2)`, `(2,3,3)`, `(0,2,4)`, `(1,3,5)`, the first three connect all vertices with total weight `6`.

```cpp
auto mst = cp::kruskal(4, {{0, 1, 1}, {1, 2, 2}, {2, 3, 3}, {0, 2, 4}, {1, 3, 5}});
assert(mst.weight == 6 && mst.edges.size() == 3 && mst.components == 1);
auto forest = cp::kruskal(4, {{0, 1, 7}});
assert(forest.components == 3);
```

**TC/SC:** `O(V+E log(E+1))` time and `O(V+E)` auxiliary space including the sorted edge copy. Supply each undirected edge once. Disconnected input produces a minimum spanning **forest**, not a failed or magically completed spanning tree. Negative edge weights are allowed.

## 3. Bipartite coloring

**Use when:** every edge requires its endpoints to belong to opposite groups.

Color a component by BFS, assigning every neighbor the opposite color. An edge joining equal colors exposes an odd cycle, which cannot alternate consistently.

```cpp
auto colors = cp::bipartite_coloring({{1, 3}, {0, 2}, {1, 3}, {0, 2}});
assert(colors && *colors == std::vector<int>({0, 1, 0, 1}));
assert(!cp::bipartite_coloring({{1, 2}, {0, 2}, {0, 1}}));
```

The four-cycle alternates `0,1,0,1`. The triangle requires its first vertex to be both colors.

**TC/SC:** `O(V+E)` time, `O(V)` workspace, plus returned colors. The routine handles disconnected undirected graphs. A self-loop is immediately incompatible with bipartiteness.

## 4. Strongly connected components: Kosaraju

**Use when:** directed reachability cycles should be collapsed into acyclic components.

An SCC is a maximal set where every vertex reaches every other. Kosaraju first computes finishing order on the original graph, then explores the reversed graph in reverse finishing order. Each second-pass traversal extracts one SCC.

For `0<->1`, `1->2`, `2<->3`, `3->4`, the components are `{0,1}`, `{2,3}`, `{4}`. Collapsing them yields a chain.

```cpp
auto scc = cp::strongly_connected_components({{1}, {0, 2}, {3}, {2, 4}, {}});
assert(scc.count == 3);
assert(scc.component[0] == scc.component[1]);
assert(scc.component[2] == scc.component[3]);
assert(scc.component[1] < scc.component[2]);
assert(scc.component[3] < scc.component[4]);
```

**TC/SC:** `O(V+E)` time and space, including the reversed graph. Both passes are iterative. Component IDs in this implementation follow topological order of the condensation DAG; arbitrary SCC implementations need not use the same numbering convention.

## 5. 2-SAT

**Use when:** constraints are conjunctions of clauses containing two Boolean literals.

A clause `(a OR b)` is equivalent to implications `NOT a -> b` and `NOT b -> a`. Build a directed implication graph with one vertex for each literal and its negation.

If a variable and its negation share an SCC, each implies the other, so no satisfying assignment exists. Otherwise the topological ordering of SCCs gives a consistent assignment.

Example clauses `(x OR y)`, `(NOT x OR y)`, `(x OR NOT y)` force both variables true.

```cpp
cp::TwoSAT sat(2);
sat.add_or(0, true, 1, true);
sat.add_or(0, false, 1, true);
sat.add_or(0, true, 1, false);
auto assignment = sat.solve();
assert(assignment && (*assignment)[0] && (*assignment)[1]);

cp::TwoSAT impossible(1);
impossible.add_or(0, true, 0, true);   // Force x.
impossible.add_or(0, false, 0, false); // Force NOT x.
assert(!impossible.solve());
```

**TC/SC:** `O(variables+clauses)` time and space. Each clause adds two edges, and SCC processing is linear. `add_or(x,value,...)` represents the literal `x==value`; it is not an implication API. General three-literal SAT is not solved by this construction.

## 6. Bridges and articulation points

**Use when:** deleting one edge or vertex might disconnect an undirected graph.

During DFS, `tin[v]` records discovery time and `low[v]` records the earliest ancestor reachable from `v`'s subtree through tree edges followed by a back edge.

For a DFS tree edge `v->u`:

- It is a bridge if `low[u] > tin[v]`: the child subtree cannot get back to `v` or an earlier ancestor without that edge.
- A non-root `v` is an articulation point if some child has `low[u] >= tin[v]`: removing `v` separates that child subtree from above.
- A root is an articulation point exactly when it has more than one DFS child.

Take a triangle `0-1-2-0` with a tail `1-3-4`. The triangle has alternative routes; only tail edges are bridges. Vertices `1` and `3` are articulation points.

```cpp
auto result = cp::bridges_and_articulations(5, {{0, 1}, {1, 2}, {2, 0}, {1, 3}, {3, 4}});
std::sort(result.bridge_ids.begin(), result.bridge_ids.end());
assert((result.bridge_ids == std::vector<int>{3, 4}));
assert(result.articulation[1] && result.articulation[3]);
assert(!result.articulation[0]);
assert(cp::bridges_and_articulations(2, {{0, 1}, {0, 1}}).bridge_ids.empty());
```

**TC/SC:** `O(V+E)` time and space, using iterative DFS. Bridge IDs refer to positions in the supplied edge list; their returned order is not a sorted-order promise.

**Critical pitfall:** skip the parent **edge ID**, not every edge to the parent vertex. A parallel edge is a valid alternative route and prevents the first edge from being a bridge. The implementation also supports disconnected graphs and self-loops.
