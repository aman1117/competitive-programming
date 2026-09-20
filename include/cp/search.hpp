#pragma once
#include "strings.hpp"

namespace cp {
// Subsets by positions (equal input values remain separate choices).
// TC O(2^n + total callback work), recursion/path SC O(n).
// Copying every emitted subset adds O(n*2^n) time and output storage.
// Callback must not retain a reference to the temporary path.
template<class Visit>
void enumerate_subsets(const std::vector<i64>& a, Visit visit) {
    std::vector<i64> path;
    auto dfs = [&](auto&& self, int start) -> void {
        visit(path);
        for (int i = start; i < static_cast<int>(a.size()); ++i) {
            path.push_back(a[i]);
            self(self, i + 1);
            path.pop_back();
        }
    };
    dfs(dfs, 0);
}

// Unique permutations, lexicographic order, duplicate values supported.
// TC O(n log n + n*P + callback work), P<=n!; SC O(n) input copy.
template<class Visit>
void enumerate_permutations(std::vector<i64> a, Visit visit) {
    std::sort(a.begin(), a.end());
    do { visit(a); } while (std::next_permutation(a.begin(), a.end()));
}

// Includes both mask itself and zero. TC O(2^popcount(mask) * callback_cost),
// SC O(1) apart from callback storage.
template<class Visit>
void enumerate_submasks(std::uint64_t mask, Visit visit) {
    std::uint64_t sub = mask;
    while (true) {
        visit(sub);
        if (sub == 0) break; // Essential: (0-1)&mask otherwise wraps back to mask.
        sub = (sub - 1) & mask;
    }
}

// TC O(2^n), SC O(1) excluding O(2^n) output. Includes empty subset.
inline std::vector<i64> subset_sums(const std::vector<i64>& a) {
    assert(a.size() <= 24); // Resource guard; intended for one MITM half.
    std::vector<i64> sums{0};
    sums.reserve(std::size_t{1} << a.size());
    for (i64 value : a) {
        std::size_t previous = sums.size();
        for (std::size_t i = 0; i < previous; ++i) sums.push_back(sums[i] + value);
    }
    return sums;
}

// Meet in the middle: count subsets summing to target, including empty subset.
// Negative values and duplicates supported. n<=40 is the practical target.
// TC O(n*2^(ceil(n/2))), SC O(2^(ceil(n/2))) for sums of both halves.
// Note 1: Split n decisions into two lists of roughly 2^(n/2) sums, sort one,
// and binary-search matching complements; do not enumerate all 2^n subsets.
inline i64 count_subsets_sum(const std::vector<i64>& a, i64 target) {
    assert(a.size() <= 40);
    auto middle = a.begin() + static_cast<std::ptrdiff_t>(a.size() / 2);
    auto left = subset_sums(std::vector<i64>(a.begin(), middle));
    auto right = subset_sums(std::vector<i64>(middle, a.end()));
    std::sort(right.begin(), right.end());
    i64 result = 0;
    for (i64 x : left) {
        auto range = std::equal_range(right.begin(), right.end(), target - x);
        result += range.second - range.first;
    }
    return result;
}

// Inversion count AND sort in place. Equal values are not inversions.
// TC O(n log n), SC O(n) merge buffer + O(log n) recursion.
inline i64 count_inversions(std::vector<i64>& a) {
    std::vector<i64> buffer(a.size());
    auto solve = [&](auto&& self, int l, int r) -> i64 {
        if (r - l <= 1) return 0;
        int m = l + (r - l) / 2;
        i64 count = self(self, l, m) + self(self, m, r);
        int i = l, j = m, k = l;
        while (i < m || j < r) {
            if (j == r || (i < m && a[i] <= a[j])) buffer[k++] = a[i++];
            else { buffer[k++] = a[j++]; count += m - i; }
        }
        std::copy(buffer.begin() + l, buffer.begin() + r, a.begin() + l);
        return count;
    };
    return solve(solve, 0, static_cast<int>(a.size()));
}
// Positive candidates, target>=0, unique VALUE combinations. reuse=true allows
// unlimited copies; reuse=false uses each input occurrence at most once.
// SC O(n+h) excluding output, h=max combination length (<=target/min_value).
// One-use TC O(n*2^n + output_size). Unlimited TC conservatively
// O(n log n + n*C(n+h,h) + output_size): at most C(n+h,h) nondecreasing paths,
// each scans <=n next candidates. This is exponential, NOT O(n*target).
inline std::vector<std::vector<i64>> combination_sum(std::vector<i64> candidates,
                                                      i64 target, bool reuse = true) {
    assert(target >= 0);
    for (i64 x : candidates) { assert(x > 0); (void)x; }
    std::sort(candidates.begin(), candidates.end());
    if (reuse) candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
    std::vector<std::vector<i64>> result;
    std::vector<i64> path;
    auto dfs = [&](auto&& self, int start, i64 remaining) -> void {
        if (remaining == 0) { result.push_back(path); return; }
        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            if (candidates[i] > remaining) break;
            if (i > start && candidates[i] == candidates[i - 1]) continue;
            path.push_back(candidates[i]);
            self(self, reuse ? i : i + 1, remaining - candidates[i]);
            path.pop_back();
        }
    };
    dfs(dfs, 0, target);
    return result;
}

// n pairs of parentheses. TC O(n*C_n + callback_work), SC O(n) recursion/path,
// C_n = Catalan number (number of valid outputs). n=0 emits one empty string.
// Callbacks must copy data they keep; the path reference is reused.
template<class Visit>
void enumerate_parentheses(int n, Visit visit) {
    assert(n >= 0);
    std::string path;
    auto dfs = [&](auto&& self, int opened, int closed) -> void {
        if (closed == n) { visit(path); return; }
        if (opened < n) { path += '('; self(self, opened + 1, closed); path.pop_back(); }
        if (closed < opened) { path += ')'; self(self, opened, closed + 1); path.pop_back(); }
    };
    dfs(dfs, 0, 0);
}

// Enumerate palindromic partitions using a precomputed table.
// TC O(n^2+n*2^n+callback_work), SC O(n^2+n), excluding retained output.
// Every possible set of cuts is a subset of n-1 boundaries; strings are copied
// into the current path. Empty string emits one empty partition.
template<class Visit>
void enumerate_palindrome_partitions(const std::string& s, Visit visit) {
    auto pal = palindrome_table(s);
    int n = static_cast<int>(s.size());
    std::vector<std::string> path;
    auto dfs = [&](auto&& self, int start) -> void {
        if (start == n) { visit(path); return; }
        for (int end = start; end < n; ++end) if (pal[start][end]) {
            path.push_back(s.substr(start, end - start + 1));
            self(self, end + 1);
            path.pop_back();
        }
    };
    dfs(dfs, 0);
}

// Four-direction word search without reusing a cell. Rectangular byte grid;
// empty word -> true. Does not modify the input grid.
// For L>0: TC O(RC*4*3^(L-1)+R), SC O(RC+L); after the first move there
// are at most 3 next choices, since the previous cell is already used.
inline bool word_exists(const std::vector<std::string>& grid, const std::string& word) {
    int rows = static_cast<int>(grid.size()), cols = rows ? static_cast<int>(grid[0].size()) : 0;
    for (const auto& row : grid) assert(static_cast<int>(row.size()) == cols);
    if (word.empty()) return true;
    if (word.size() > static_cast<std::size_t>(rows) * cols) return false;
    std::vector<std::vector<char>> used(rows, std::vector<char>(cols));
    constexpr int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    auto dfs = [&](auto&& self, int r, int c, int position) -> bool {
        if (r < 0 || r >= rows || c < 0 || c >= cols || used[r][c] || grid[r][c] != word[position]) return false;
        if (position + 1 == static_cast<int>(word.size())) return true;
        used[r][c] = true;
        bool found = false;
        for (int d = 0; d < 4 && !found; ++d) found = self(self, r + dr[d], c + dc[d], position + 1);
        used[r][c] = false; // Restore on success as well as failure.
        return found;
    };
    for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c)
        if (dfs(dfs, r, c, 0)) return true;
    return false;
}
// Multiword grid search: lowercase a-z grid and NONEMPTY dictionary words;
// duplicates produce one output word, output order unspecified. Input unchanged.
// Shared trie prefixes prune impossible paths; each word is emitted at most once.
// TC O(S+w+R+RC*4*3^(L-1)+output_chars), L=max word length (L>=1),
// S=total dictionary characters, w=word count; SC O(26S+RC+L).
// Empty dictionary returns no words. Prefix pruning helps in practice but does
// NOT remove the exponential worst case. Recursion depth <=min(RC,L).
inline std::vector<std::string> find_words(const std::vector<std::string>& grid,
                                         const std::vector<std::string>& dictionary) {
    Trie trie;
    std::vector<int> word_at(1, -1);
    for (int i = 0; i < static_cast<int>(dictionary.size()); ++i) {
        assert(!dictionary[i].empty());
        int node = trie.insert(dictionary[i]);
        if (node >= static_cast<int>(word_at.size())) word_at.resize(node + 1, -1);
        word_at[node] = i;
    }
    int rows = static_cast<int>(grid.size()), cols = rows ? static_cast<int>(grid[0].size()) : 0;
    for (const auto& row : grid) assert(static_cast<int>(row.size()) == cols);
    if (dictionary.empty() || !rows || !cols) return {};
    std::vector<std::vector<char>> used(rows, std::vector<char>(cols));
    std::vector<std::string> answer;
    constexpr int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    auto dfs = [&](auto&& self, int r, int c, int parent) -> void {
        if (r < 0 || r >= rows || c < 0 || c >= cols || used[r][c]) return;
        int node = trie.transition(parent, grid[r][c]);
        if (node == -1) return;
        if (word_at[node] != -1) {
            answer.push_back(dictionary[word_at[node]]);
            word_at[node] = -1;
        }
        used[r][c] = true;
        for (int d = 0; d < 4; ++d) self(self, r + dr[d], c + dc[d], node);
        used[r][c] = false;
    };
    for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c) dfs(dfs, r, c, 0);
    return answer;
}

// N-Queens, emits column index for each row; n=0 emits one empty placement.
// Bitmasks prune occupied columns/diagonals. TC O(n*n! + callback_work) is a
// conservative bound (column extraction O(n) per state), SC O(n).
// Exponential: n<=15 is the intended practical limit; mask supports up to 20.
template<class Visit>
void enumerate_n_queens(int n, Visit visit) {
    assert(0 <= n && n <= 20);
    std::uint32_t all = (std::uint32_t{1} << n) - 1;
    std::vector<int> columns;
    auto dfs = [&](auto&& self, std::uint32_t used, std::uint32_t down, std::uint32_t up) -> void {
        if (used == all) { visit(columns); return; }
        auto available = all & ~(used | down | up);
        while (available) {
            auto bit = available & (std::uint32_t{0} - available);
            available ^= bit;
            int column = 0;
            for (auto x = bit; x > 1; x >>= 1) ++column;
            columns.push_back(column);
            self(self, used | bit, ((down | bit) << 1) & all, (up | bit) >> 1);
            columns.pop_back();
        }
    };
    dfs(dfs, 0, 0, 0);
}
} // namespace cp
