#pragma once
#include "data_structures.hpp"

namespace cp {
using Graph = std::vector<std::vector<int>>;
struct WeightedEdge { int to; i64 weight; };
using WeightedGraph = std::vector<std::vector<WeightedEdge>>;
struct Edge { int from, to; i64 weight; };
struct BFSResult { std::vector<int> distance, parent; };

// Single/multi-source BFS, unweighted graph; unreachable distance = -1.
// TC O(V+E+S), S=source-list length (duplicates allowed); SC O(V) queue,
// excluding O(V) output. For one source, the usual bound is O(V+E).
inline BFSResult bfs(const Graph& g, const std::vector<int>& sources) {
    int n = static_cast<int>(g.size());
    BFSResult result{std::vector<int>(n, -1), std::vector<int>(n, -1)};
    std::queue<int> q;
    for (int s : sources) {
        assert(0 <= s && s < n);
        if (result.distance[s] == -1) {
            result.distance[s] = 0;
            q.push(s);
        }
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : g[v]) if (result.distance[u] == -1) {
            result.distance[u] = result.distance[v] + 1;
            result.parent[u] = v;
            q.push(u);
        }
    }
    return result;
}

// Recover path from a BFS/Dijkstra predecessor array; empty if target not rooted
// at source. Parent array must be an acyclic predecessor forest.
// TC O(predecessor chain length), SC O(1) excluding output.
inline std::vector<int> restore_path(const std::vector<int>& parent, int source, int target) {
    assert(source >= 0 && source < static_cast<int>(parent.size()));
    assert(target >= 0 && target < static_cast<int>(parent.size()));
    int root = target, length = 1;
    while (root != -1 && root != source) { root = parent[root]; ++length; }
    if (root == -1) return {};
    // Confirm reachability before allocating: an unreachable chain must not
    // consume O(V) temporary memory while returning an empty output.
    std::vector<int> path;
    path.reserve(length);
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
        if (v == source) {
            std::reverse(path.begin(), path.end());
            return path;
        }
    }
    return {};
}

// Recursive-equivalent DFS preorder in adjacency-list order, implemented with
// explicit frames. TC O(V+E), SC O(V); safe for deep graphs.
inline std::vector<int> dfs_order(const Graph& g, int source) {
    assert(source >= 0 && source < static_cast<int>(g.size()));
    std::vector<char> seen(g.size());
    std::vector<int> stack{source}, order{source}, next(g.size());
    seen[source] = true;
    while (!stack.empty()) {
        int v = stack.back();
        if (next[v] == static_cast<int>(g[v].size())) { stack.pop_back(); continue; }
        int u = g[v][next[v]++];
        // Discover ONE child before continuing siblings. Eagerly marking all
        // siblings can produce an order that is not a true DFS preorder.
        if (!seen[u]) { seen[u] = true; order.push_back(u); stack.push_back(u); }
    }
    return order;
}

// Undirected graph (including disconnected components), nullopt for odd cycle.
// TC O(V+E), SC O(V) queue; returned colors are 0 or 1.
inline std::optional<std::vector<int>> bipartite_coloring(const Graph& g) {
    int n = static_cast<int>(g.size());
    std::vector<int> color(n, -1);
    std::queue<int> q;
    for (int s = 0; s < n; ++s) if (color[s] == -1) {
        color[s] = 0; q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u : g[v]) {
                if (color[u] == -1) { color[u] = color[v] ^ 1; q.push(u); }
                else if (color[u] == color[v]) return std::nullopt;
            }
        }
    }
    return color;
}

// Kahn's topological sort of a directed graph; nullopt means a cycle exists.
// TC O(V+E), SC O(V). Each edge decreases indegree exactly once.
inline std::optional<std::vector<int>> topological_sort(const Graph& g) {
    std::vector<int> indegree(g.size()), order;
    for (const auto& row : g) for (int u : row) ++indegree[u];
    std::queue<int> q;
    for (int v = 0; v < static_cast<int>(g.size()); ++v) if (!indegree[v]) q.push(v);
    while (!q.empty()) {
        int v = q.front(); q.pop(); order.push_back(v);
        for (int u : g[v]) if (--indegree[u] == 0) q.push(u);
    }
    if (order.size() != g.size()) return std::nullopt;
    return order;
}

struct ShortestPaths { std::vector<i64> distance; std::vector<int> parent; };
// NONNEGATIVE edge weights only. Unreachable = INF.
// Lazy binary heap: TC O(V + E log(E+1)), SC O(V+E), not O(V) heap space.
// On simple graphs, this is commonly written O((V+E) log V).
inline ShortestPaths dijkstra(const WeightedGraph& g, int source) {
    int n = static_cast<int>(g.size());
    assert(0 <= source && source < n);
    ShortestPaths result{std::vector<i64>(n, INF), std::vector<int>(n, -1)};
    using State = std::pair<i64, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> q;
    result.distance[source] = 0; q.emplace(0, source);
    while (!q.empty()) {
        auto [distance, v] = q.top(); q.pop();
        // Note 1: Outdated entries must be skipped or repeated edge scans can
        // destroy the stated bound. Each improvement creates at most one entry.
        if (distance != result.distance[v]) continue;
        for (auto [u, weight] : g[v]) {
            assert(0 <= weight && weight < INF);
            if (distance + weight < result.distance[u]) {
                result.distance[u] = distance + weight;
                result.parent[u] = v;
                q.emplace(result.distance[u], u);
            }
        }
    }
    return result;
}

// Edge weights exactly 0 or 1. TC O(V+E), SC O(V+E) for lazy deque entries.
inline std::vector<i64> zero_one_bfs(const WeightedGraph& g, int source) {
    assert(0 <= source && source < static_cast<int>(g.size()));
    std::vector<i64> distance(g.size(), INF);
    std::deque<std::pair<i64, int>> q;
    distance[source] = 0; q.emplace_front(0, source);
    while (!q.empty()) {
        auto [d, v] = q.front(); q.pop_front();
        if (d != distance[v]) continue;
        for (auto [u, w] : g[v]) {
            assert(w == 0 || w == 1);
            if (d + w < distance[u]) {
                distance[u] = d + w;
                if (w == 0) q.emplace_front(distance[u], u);
                else q.emplace_back(distance[u], u);
            }
        }
    }
    return distance;
}

struct BellmanFordResult {
    std::vector<i64> distance;
    // true means no finite shortest distance: reachable from a reachable negative cycle.
    std::vector<char> negative_infinite;
};
// Directed edge list; duplicate edges allowed. TC O(VE+V+E), SC O(V+E).
// Arithmetic contract: even repeated relaxations must stay inside (-INF,INF).
// A sufficient conservative bound is (V*E+1)*max_abs_weight < INF.
inline BellmanFordResult bellman_ford(int n, const std::vector<Edge>& edges, int source) {
    assert(0 <= source && source < n);
    BellmanFordResult result{std::vector<i64>(n, INF), std::vector<char>(n)};
    result.distance[source] = 0;
    for (int pass = 1; pass < n; ++pass) {
        bool changed = false;
        for (auto [v, u, w] : edges)
            if (result.distance[v] != INF && result.distance[v] + w < result.distance[u]) {
                result.distance[u] = result.distance[v] + w;
                changed = true;
            }
        if (!changed) break;
    }
    Graph g(n);
    std::queue<int> q;
    for (auto [v, u, w] : edges) {
        g[v].push_back(u);
        if (result.distance[v] != INF && result.distance[v] + w < result.distance[u]
            && !result.negative_infinite[u]) {
            result.negative_infinite[u] = true;
            q.push(u);
        }
    }
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : g[v]) if (!result.negative_infinite[u]) {
            result.negative_infinite[u] = true; q.push(u);
        }
    }
    return result;
}

// All-pairs shortest paths. Initialize diagonal 0, absent edges INF, parallel
// edges to minimum weight. REQUIRES no negative cycles; use Bellman-Ford if unsure.
// TC O(V^3), SC O(V^2) by-value matrix workspace (also returned).
inline std::vector<std::vector<i64>> floyd_warshall(std::vector<std::vector<i64>> d) {
    int n = static_cast<int>(d.size());
    for (const auto& row : d) assert(static_cast<int>(row.size()) == n);
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i) if (d[i][k] != INF)
            for (int j = 0; j < n; ++j) if (d[k][j] != INF)
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    return d;
}

struct MSTResult { i64 weight; std::vector<Edge> edges; int components; };
// Undirected graph: supply each edge once. Disconnected -> minimum spanning
// FOREST; result.components > 1 signals no spanning tree.
// TC O(V+E log(E+1)), SC O(V+E) including by-value edge copy and DSU.
inline MSTResult kruskal(int n, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](auto a, auto b) { return a.weight < b.weight; });
    DSU dsu(n);
    MSTResult result{0, {}, n};
    for (auto e : edges) if (dsu.unite(e.from, e.to)) {
        result.weight += e.weight;
        result.edges.push_back(e);
    }
    result.components = dsu.components();
    return result;
}
// Shortest unweighted WALK visiting all vertices, any start/end, revisits allowed.
// Directed or undirected graph; -1 if impossible; n<=1 -> 0.
// State=(vertex,visited_mask), NOT just vertex. TC O((V+E)*2^V), SC O(V*2^V).
// Intended for V<=15; V<=20 guard still permits very memory-heavy inputs.
inline int shortest_visit_all(const Graph& g) {
    int n = static_cast<int>(g.size());
    assert(n <= 20);
    if (n <= 1) return 0;
    int states = 1 << n;
    std::vector<std::vector<int>> distance(n, std::vector<int>(states, -1));
    std::queue<std::pair<int, int>> q;
    for (int v = 0; v < n; ++v) { distance[v][1 << v] = 0; q.emplace(v, 1 << v); }
    while (!q.empty()) {
        auto [v, mask] = q.front(); q.pop();
        if (mask == states - 1) return distance[v][mask];
        for (int u : g[v]) {
            int next_mask = mask | (1 << u);
            if (distance[u][next_mask] == -1) {
                distance[u][next_mask] = distance[v][mask] + 1;
                q.emplace(u, next_mask);
            }
        }
    }
    return -1;
}
} // namespace cp
