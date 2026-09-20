#pragma once
#include "graphs.hpp"

namespace cp {
// Connected, nonempty, undirected tree; no self-loops/parallel edges.
class BinaryLifting {
    int n, levels = 1;
    std::vector<std::vector<int>> up;
public:
    std::vector<int> parent, depth, tin, tout, order, subtree_size;
    // TC/space O(n log n), including table and iterative traversal.
    // A subtree is exactly [tin[v],tout[v]) in preorder.
    explicit BinaryLifting(const Graph& tree, int root = 0)
        : n(static_cast<int>(tree.size())), parent(n, -1), depth(n),
          tin(n), tout(n), subtree_size(n, 1) {
        assert(n > 0 && root >= 0 && root < n);
        while ((1LL << levels) <= n) ++levels;
        up.assign(levels, std::vector<int>(n));
        std::vector<int> stack{root};
        parent[root] = root;
        while (!stack.empty()) {
            int v = stack.back(); stack.pop_back();
            tin[v] = static_cast<int>(order.size());
            order.push_back(v);
            for (int u : tree[v]) if (u != parent[v]) {
                assert(parent[u] == -1);
                parent[u] = v; depth[u] = depth[v] + 1; stack.push_back(u);
            }
        }
        assert(static_cast<int>(order.size()) == n);
        for (int i = n - 1; i > 0; --i) {
            int v = order[i];
            subtree_size[parent[v]] += subtree_size[v];
        }
        for (int v = 0; v < n; ++v) tout[v] = tin[v] + subtree_size[v];
        up[0] = parent;
        for (int k = 1; k < levels; ++k)
            for (int v = 0; v < n; ++v) up[k][v] = up[k - 1][up[k - 1][v]];
    }
    // TC O(log n), SC O(1). Returns -1 above root, not root forever.
    int kth_ancestor(int v, int k) const {
        assert(0 <= v && v < n && k >= 0);
        if (k > depth[v]) return -1;
        for (int b = 0; b < levels; ++b) if ((k >> b) & 1) v = up[b][v];
        return v;
    }
    // TC/SC O(1).
    bool is_ancestor(int a, int b) const {
        assert(0 <= a && a < n && 0 <= b && b < n);
        return tin[a] <= tin[b] && tin[b] < tout[a];
    }
    // TC O(log n), SC O(1).
    int lca(int a, int b) const {
        if (is_ancestor(a, b)) return a;
        if (is_ancestor(b, a)) return b;
        for (int k = levels - 1; k >= 0; --k)
            if (!is_ancestor(up[k][a], b)) a = up[k][a];
        return parent[a];
    }
    // Unweighted edge distance. TC O(log n), SC O(1).
    int distance(int a, int b) const { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};

struct Diameter { int from, to, length; std::vector<int> path; };
// Unweighted nonempty tree only, not a general graph. TC/SC O(n).
inline Diameter tree_diameter(const Graph& tree) {
    assert(!tree.empty());
    auto first = bfs(tree, {0});
    int a = static_cast<int>(std::max_element(first.distance.begin(), first.distance.end()) - first.distance.begin());
    auto second = bfs(tree, {a});
    int b = static_cast<int>(std::max_element(second.distance.begin(), second.distance.end()) - second.distance.begin());
    return {a, b, second.distance[b], restore_path(second.parent, a, b)};
}

// Sum of distances from EACH vertex of a nonempty unweighted tree.
// TC/SC O(n), using iterative parent-order traversal and rerooting.
inline std::vector<i64> tree_distance_sums(const Graph& tree) {
    int n = static_cast<int>(tree.size());
    assert(n > 0);
    auto traversal = bfs(tree, {0});
    std::vector<int> order{0}, size(n, 1);
    for (int i = 0; i < n; ++i)
        for (int u : tree[order[i]]) if (traversal.parent[u] == order[i]) order.push_back(u);
    std::vector<i64> answer(n);
    for (int d : traversal.distance) answer[0] += d;
    for (int i = n - 1; i > 0; --i) size[traversal.parent[order[i]]] += size[order[i]];
    for (int i = 1; i < n; ++i) {
        int v = order[i], p = traversal.parent[v];
        // Note 1: Moving p -> v shortens distances to size[v] vertices by 1
        // and lengthens distances to the other n-size[v] vertices by 1.
        answer[v] = answer[p] + n - 2LL * size[v];
    }
    return answer;
}

// Maximum-weight independent set of a nonempty tree; choosing nothing allowed.
// TC/SC O(n). dp[v][1] takes v, forcing all children to be skipped.
inline i64 tree_independent_set(const Graph& tree, const std::vector<i64>& weight) {
    int n = static_cast<int>(tree.size());
    assert(n > 0 && weight.size() == tree.size());
    auto traversal = bfs(tree, {0});
    std::vector<int> order{0};
    for (int i = 0; i < n; ++i)
        for (int u : tree[order[i]]) if (traversal.parent[u] == order[i]) order.push_back(u);
    std::vector<std::array<i64, 2>> dp(n);
    for (int i = n - 1; i >= 0; --i) {
        int v = order[i];
        dp[v][1] = weight[v];
        for (int u : tree[v]) if (traversal.parent[u] == v) {
            dp[v][0] += std::max(dp[u][0], dp[u][1]);
            dp[v][1] += dp[u][0];
        }
    }
    return std::max(dp[0][0], dp[0][1]);
}
} // namespace cp
