#pragma once
#include "common.hpp"

namespace cp {
/// Four-direction multi-source grid BFS. '#' is blocked; every other character
/// is traversable. Sources must be in bounds and unblocked. Distances -1 if
/// unreachable. Rectangular grid. TC O(R*(C+1)+S), SC O(RC) queue excluding
/// O(R*(C+1)) output. For positive C, these simplify to O(RC+S) / O(RC).
inline std::vector<std::vector<int>> grid_bfs(
    const std::vector<std::string>& grid, const std::vector<std::pair<int, int>>& sources) {
    int rows = static_cast<int>(grid.size()), cols = rows ? static_cast<int>(grid[0].size()) : 0;
    for (const auto& row : grid) assert(static_cast<int>(row.size()) == cols);
    std::vector<std::vector<int>> distance(rows, std::vector<int>(cols, -1));
    std::queue<std::pair<int, int>> q;
    for (auto [r, c] : sources) {
        assert(0 <= r && r < rows && 0 <= c && c < cols && grid[r][c] != '#');
        if (distance[r][c] == -1) { distance[r][c] = 0; q.emplace(r, c); }
    }
    constexpr int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == '#' || distance[nr][nc] != -1) continue;
            distance[nr][nc] = distance[r][c] + 1;
            q.emplace(nr, nc);
        }
    }
    return distance;
}

/// Longest substring with no repeated BYTE (not Unicode code points).
/// TC O(n+256), SC O(256)=O(1). last[] lets the left boundary jump rather than
/// repeatedly erase characters. max avoids moving the boundary backward.
inline int longest_unique_substring(const std::string& s) {
    std::array<int, 256> last;
    last.fill(-1);
    int left = 0, best = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        auto c = static_cast<unsigned char>(s[right]);
        left = std::max(left, last[c] + 1);
        last[c] = right;
        best = std::max(best, right - left + 1);
    }
    return best;
}

/// Count subarrays with exactly k distinct values via at_most(k)-at_most(k-1).
/// TC O(n log n) with deterministic std::map, SC O(n).
/// Expected O(n) using unordered_map is not a worst-case guarantee.
inline i64 subarrays_exactly_k_distinct(const std::vector<i64>& a, int k) {
    assert(k >= 0);
    auto at_most = [&](int limit) -> i64 {
        if (limit < 0) return 0;
        std::map<i64, int> frequency;
        int left = 0;
        i64 answer = 0;
        for (int right = 0; right < static_cast<int>(a.size()); ++right) {
            ++frequency[a[right]];
            while (static_cast<int>(frequency.size()) > limit) {
                auto it = frequency.find(a[left++]);
                if (--it->second == 0) frequency.erase(it);
            }
            answer += right - left + 1;
        }
        return answer;
    };
    return at_most(k) - at_most(k - 1);
}
/// Smallest [l,r) containing every BYTE of target, with multiplicity.
/// Empty target -> [0,0); impossible -> nullopt; first minimum-length tie wins.
/// TC O(n+m+256), SC O(256). Each window boundary advances at most n times.
inline std::optional<std::pair<int, int>> minimum_window(const std::string& text, const std::string& target) {
    if (target.empty()) return std::pair<int, int>{0, 0};
    std::array<int, 256> need{};
    for (unsigned char ch : target) ++need[ch];
    int missing = static_cast<int>(target.size()), left = 0;
    std::optional<std::pair<int, int>> best;
    for (int right = 0; right < static_cast<int>(text.size()); ++right) {
        if (need[static_cast<unsigned char>(text[right])]-- > 0) --missing;
        while (missing == 0) {
            if (!best || right + 1 - left < best->second - best->first) best = {left, right + 1};
            if (++need[static_cast<unsigned char>(text[left++])] > 0) ++missing;
        }
    }
    return best;
}

/// Binary input; longest window containing at most k zeros (flip those zeros).
/// TC O(n), SC O(1). This is the "at most k bad elements" window pattern.
inline int longest_ones_after_flips(const std::vector<int>& a, int k) {
    assert(k >= 0);
    int left = 0, zeros = 0, best = 0;
    for (int right = 0; right < static_cast<int>(a.size()); ++right) {
        assert(a[right] == 0 || a[right] == 1);
        zeros += a[right] == 0;
        while (zeros > k) zeros -= a[left++] == 0;
        best = std::max(best, right - left + 1);
    }
    return best;
}

/// STRICTLY POSITIVE integer values, count subarrays with product < limit.
/// TC O(n), SC O(1). Uses division to avoid overflowing before shrinking.
inline i64 count_product_less(const std::vector<i64>& a, i64 limit) {
    if (limit <= 1) return 0;
    int left = 0;
    i64 product = 1, count = 0;
    for (int right = 0; right < static_cast<int>(a.size()); ++right) {
        i64 x = a[right];
        assert(x > 0);
        if (x >= limit) { left = right + 1; product = 1; continue; }
        while (left < right && product > (limit - 1) / x) product /= a[left++];
        product *= x; // Now product <= limit-1, even if limit is LLONG_MAX.
        count += right - left + 1;
    }
    return count;
}

/// Prefix XOR frequency pattern; works because p[l]^p[r] = target iff
/// p[l] = p[r]^target. TC O(n log n), SC O(n) with deterministic map.
inline i64 count_subarrays_xor(const std::vector<std::uint32_t>& a, std::uint32_t target) {
    std::map<std::uint32_t, i64> frequency{{0, 1}};
    std::uint32_t prefix = 0;
    i64 count = 0;
    for (auto x : a) {
        prefix ^= x;
        auto it = frequency.find(prefix ^ target);
        if (it != frequency.end()) count += it->second;
        ++frequency[prefix];
    }
    return count;
}

/// Strictly increasing 4-direction path length in a rectangular matrix.
/// Treat smaller -> larger neighbors as a DAG and peel topological layers.
/// TC/SC O(R*(C+1)), normally O(RC); avoids recursive depth up to R*C.
inline int longest_increasing_path(const std::vector<std::vector<i64>>& matrix) {
    int rows = static_cast<int>(matrix.size()), cols = rows ? static_cast<int>(matrix[0].size()) : 0;
    for (const auto& row : matrix) assert(static_cast<int>(row.size()) == cols);
    std::vector<std::vector<int>> indegree(rows, std::vector<int>(cols));
    std::queue<std::pair<int, int>> q;
    constexpr int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    auto inside = [&](int r, int c) { return r >= 0 && r < rows && c >= 0 && c < cols; };
    for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c) {
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (inside(nr, nc) && matrix[nr][nc] < matrix[r][c]) ++indegree[r][c];
        }
        if (!indegree[r][c]) q.emplace(r, c);
    }
    int length = 0;
    while (!q.empty()) {
        int layer_size = static_cast<int>(q.size());
        ++length;
        while (layer_size--) {
            auto [r, c] = q.front(); q.pop();
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (inside(nr, nc) && matrix[nr][nc] > matrix[r][c] && --indegree[nr][nc] == 0)
                    q.emplace(nr, nc);
            }
        }
    }
    return length;
}
} // namespace cp
