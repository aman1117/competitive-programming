#pragma once
#include "graphs.hpp"

namespace cp {
struct SCCResult { int count; std::vector<int> component; };
/// Iterative Kosaraju; safe for long chains without a recursive call stack.
/// TC O(V+E), SC O(V+E) for reverse graph, visited arrays, traversal stacks.
/// Component IDs are in topological order of the condensation DAG.
inline SCCResult strongly_connected_components(const Graph& g) {
    int n = static_cast<int>(g.size());
    Graph reverse(n);
    for (int v = 0; v < n; ++v) for (int u : g[v]) reverse[u].push_back(v);
    std::vector<char> seen(n);
    std::vector<int> order, next(n);
    for (int s = 0; s < n; ++s) if (!seen[s]) {
        std::vector<int> stack{s};
        seen[s] = true;
        while (!stack.empty()) {
            int v = stack.back();
            if (next[v] == static_cast<int>(g[v].size())) {
                order.push_back(v); stack.pop_back();
            } else {
                int u = g[v][next[v]++];
                if (!seen[u]) { seen[u] = true; stack.push_back(u); }
            }
        }
    }
    SCCResult result{0, std::vector<int>(n, -1)};
    std::reverse(order.begin(), order.end());
    for (int s : order) if (result.component[s] == -1) {
        std::vector<int> stack{s};
        result.component[s] = result.count;
        while (!stack.empty()) {
            int v = stack.back(); stack.pop_back();
            for (int u : reverse[v]) if (result.component[u] == -1) {
                result.component[u] = result.count;
                stack.push_back(u);
            }
        }
        ++result.count;
    }
    return result;
}

/// @brief Solve conjunctions of two-literal Boolean OR clauses using SCCs.
/// Solve TC/SC O(variables+clauses); returns an assignment or nullopt.
class TwoSAT {
    int variables;
    Graph implications;
public:
    /// n Boolean variables, m OR clauses. Total solve TC/SC O(n+m).
    explicit TwoSAT(int n) : variables(n), implications(2 * n) {}
    /// Add (x == x_value) OR (y == y_value), amortized TC/SC O(1).
    void add_or(int x, bool x_value, int y, bool y_value) {
        assert(0 <= x && x < variables && 0 <= y && y < variables);
        int a = 2 * x + static_cast<int>(x_value);
        int b = 2 * y + static_cast<int>(y_value);
        implications[a ^ 1].push_back(b);
        implications[b ^ 1].push_back(a);
    }
    /// @brief Return a satisfying assignment, or nullopt if constraints conflict.
    /// A variable and its negation cannot share an SCC. TC/SC O(variables+clauses).
    std::optional<std::vector<bool>> solve() const {
        auto scc = strongly_connected_components(implications);
        std::vector<bool> assignment(variables);
        for (int i = 0; i < variables; ++i) {
            if (scc.component[2 * i] == scc.component[2 * i + 1]) return std::nullopt;
            assignment[i] = scc.component[2 * i + 1] > scc.component[2 * i];
        }
        return assignment;
    }
};

struct LowLinkResult { std::vector<int> bridge_ids; std::vector<char> articulation; };
/// Undirected edges supplied once. Handles disconnected graphs, parallel edges
/// and self-loops. TC O(V+E), SC O(V+E). Iterative DFS avoids stack overflow.
inline LowLinkResult bridges_and_articulations(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<std::pair<int, int>>> g(n);
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
        auto [u, v] = edges[i];
        g[u].emplace_back(v, i); g[v].emplace_back(u, i);
    }
    std::vector<int> tin(n, -1), low(n), parent(n, -1), parent_edge(n, -1), next(n), children(n);
    LowLinkResult result{{}, std::vector<char>(n)};
    int timer = 0;
    for (int s = 0; s < n; ++s) if (tin[s] == -1) {
        std::vector<int> stack{s};
        tin[s] = low[s] = timer++;
        while (!stack.empty()) {
            int v = stack.back();
            if (next[v] < static_cast<int>(g[v].size())) {
                auto [u, id] = g[v][next[v]++];
                // Note 1: Skip only the parent EDGE, not every edge to the
                // parent vertex; a parallel edge is a valid back edge.
                if (id == parent_edge[v]) continue;
                if (tin[u] == -1) {
                    parent[u] = v; parent_edge[u] = id; ++children[v];
                    tin[u] = low[u] = timer++;
                    stack.push_back(u);
                } else low[v] = std::min(low[v], tin[u]);
            } else {
                stack.pop_back();
                int p = parent[v];
                if (p == -1) result.articulation[v] = children[v] > 1;
                else {
                    low[p] = std::min(low[p], low[v]);
                    if (low[v] > tin[p]) result.bridge_ids.push_back(parent_edge[v]);
                    if (parent[p] != -1 && low[v] >= tin[p]) result.articulation[p] = true;
                }
            }
        }
    }
    return result;
}
} // namespace cp
