#pragma once
#include "arrays.hpp"

namespace cp {
// Mo's algorithm: number of distinct values in each STATIC [l,r) query.
// Returns answers in original query order; no point updates supported.
// TC O(n log n + q log(q+1) + (n+q)*sqrt(n+1)), SC O(n+q).
// Compression gives O(1) add/remove; using a map inside them adds log n.
inline std::vector<int> mo_distinct(const std::vector<i64>& a,
                                   const std::vector<std::pair<int, int>>& queries) {
    int n = static_cast<int>(a.size()), q = static_cast<int>(queries.size());
    auto rank = compress(a);
    std::vector<int> frequency(n), order(q), result(q);
    std::iota(order.begin(), order.end(), 0);
    for (auto [l, r] : queries) assert(0 <= l && l <= r && r <= n);
    int block = std::max(1, static_cast<int>(std::sqrt(n)));
    std::sort(order.begin(), order.end(), [&](int x, int y) {
        int bx = queries[x].first / block, by = queries[y].first / block;
        if (bx != by) return bx < by;
        return (bx & 1) ? queries[x].second > queries[y].second : queries[x].second < queries[y].second;
    });
    int left = 0, right = 0, distinct = 0;
    auto add = [&](int i) { if (frequency[rank[i]]++ == 0) ++distinct; };
    auto remove = [&](int i) { if (--frequency[rank[i]] == 0) --distinct; };
    for (int id : order) {
        auto [l, r] = queries[id];
        while (left > l) add(--left);
        while (right < r) add(right++);
        while (left < l) remove(left++);
        while (right > r) remove(--right);
        result[id] = distinct;
    }
    // Note 1: Across O(sqrt(n)) left blocks, the right pointer scans O(n) each;
    // within each block, each left-pointer adjustment costs O(sqrt(n)).
    return result;
}

class FunctionalJump {
    std::vector<std::vector<int>> up;
    std::uint64_t maximum;
public:
    // successor[v] is exactly one valid next vertex (cycles/self-loops allowed).
    // Precompute TC/space O(nB), B=max(1,bit_width(max_steps)).
    FunctionalJump(const std::vector<int>& successor, std::uint64_t max_steps)
        : maximum(max_steps) {
        int n = static_cast<int>(successor.size()), levels = 1;
        for (auto x = max_steps; x > 1; x >>= 1) ++levels;
        for (int v : successor) assert(0 <= v && v < n);
        up.assign(levels, successor);
        for (int b = 1; b < levels; ++b)
            for (int v = 0; v < n; ++v) up[b][v] = up[b - 1][up[b - 1][v]];
    }
    // TC O(B), SC O(1). Require steps <= precomputed maximum.
    int jump(int vertex, std::uint64_t steps) const {
        assert(vertex >= 0 && vertex < static_cast<int>(up[0].size()) && steps <= maximum);
        for (int b = 0; steps; ++b, steps >>= 1) if (steps & 1U) vertex = up[b][vertex];
        return vertex;
    }
};
} // namespace cp
