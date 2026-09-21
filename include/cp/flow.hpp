#pragma once
#include "graphs.hpp"

namespace cp {
/// Stretch topic: integral-capacity directed maximum flow, with residual edges.
class Dinic {
    struct Arc { int to, reverse; i64 capacity; };
    int n;
    std::vector<std::vector<Arc>> g;
    std::vector<int> level, next;
    bool build_levels(int source, int sink) {
        std::fill(level.begin(), level.end(), -1);
        std::queue<int> q;
        level[source] = 0; q.push(source);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto e : g[v]) if (e.capacity > 0 && level[e.to] == -1) {
                level[e.to] = level[v] + 1; q.push(e.to);
            }
        }
        return level[sink] != -1;
    }
    i64 send(int v, int sink, i64 amount) {
        if (v == sink) return amount;
        for (int& i = next[v]; i < static_cast<int>(g[v].size()); ++i) {
            auto& edge = g[v][i];
            if (!edge.capacity || level[edge.to] != level[v] + 1) continue;
            i64 pushed = send(edge.to, sink, std::min(amount, edge.capacity));
            if (!pushed) continue;
            edge.capacity -= pushed;
            g[edge.to][edge.reverse].capacity += pushed;
            return pushed;
        }
        return 0;
    }
public:
    /// Total stored space O(V+E). DFS recursion O(V): very deep level graphs
    /// may exhaust the native stack; use an iterative flow implementation there.
    explicit Dinic(int vertices) : n(vertices), g(vertices), level(vertices), next(vertices) {}
    /// Amortized TC O(1), space O(1) per edge. Parallel edges allowed; self-loops
    /// excluded (they never improve s-t flow). Capacities and total flow < INF.
    void add_edge(int from, int to, i64 capacity) {
        assert(0 <= from && from < n && 0 <= to && to < n && from != to);
        assert(0 <= capacity && capacity < INF);
        int a = static_cast<int>(g[from].size()), b = static_cast<int>(g[to].size());
        g[from].push_back({to, b, capacity});
        g[to].push_back({from, a, 0});
    }
    /// General-case TC O(V^2 E), auxiliary SC O(V).
    /// Note 1: At most V level phases; current-arc pointers bound each blocking
    /// flow by O(VE). Do not claim the faster unit-capacity bound for all graphs.
    /// Mutates residual capacities; a second call returns only ADDITIONAL flow.
    i64 max_flow(int source, int sink) {
        assert(0 <= source && source < n && 0 <= sink && sink < n && source != sink);
        i64 flow = 0;
        while (build_levels(source, sink)) {
            std::fill(next.begin(), next.end(), 0);
            while (i64 amount = send(source, sink, INF)) flow += amount;
        }
        return flow;
    }
    /// After max_flow, reachable vertices form source side of a minimum cut.
    /// TC O(V+E), SC O(V), including returned reachability flags.
    std::vector<char> min_cut_side(int source) const {
        assert(0 <= source && source < n);
        std::vector<char> seen(n);
        std::queue<int> q;
        seen[source] = true; q.push(source);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto e : g[v]) if (e.capacity && !seen[e.to]) {
                seen[e.to] = true; q.push(e.to);
            }
        }
        return seen;
    }
};

/// Kuhn augmenting-path bipartite matching. g[left] lists right-side indices.
/// Output right_match[r] = matched left vertex, or -1.
/// TC O(L*(L+E)+R), SC O(L+R), including recursion and output.
/// Put the smaller partition on the left; Hopcroft-Karp is a later upgrade.
inline std::vector<int> bipartite_matching(const Graph& g, int right_count) {
    std::vector<int> match(right_count, -1), seen(g.size(), -1);
    int stamp = 0;
    auto augment = [&](auto&& self, int v) -> bool {
        if (seen[v] == stamp) return false;
        seen[v] = stamp;
        for (int r : g[v]) {
            assert(0 <= r && r < right_count);
            if (match[r] == -1 || self(self, match[r])) {
                match[r] = v;
                return true;
            }
        }
        return false;
    };
    for (int v = 0; v < static_cast<int>(g.size()); ++v) { ++stamp; augment(augment, v); }
    return match;
}
} // namespace cp
