#pragma once
#include "common.hpp"

namespace cp {
// pi[i] = longest proper prefix of s[0..i] that is also a suffix.
// TC O(n), SC O(1) excluding output. The matched length can increase only n
// times; all fallback decreases together are O(n), despite the nested while.
inline std::vector<int> prefix_function(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// KMP finds all occurrences, including overlaps. Empty pattern matches n+1
// boundaries. TC O(n+m), SC O(m) prefix table, excluding O(matches) output.
inline std::vector<int> kmp_search(const std::string& text, const std::string& pattern) {
    int n = static_cast<int>(text.size()), m = static_cast<int>(pattern.size());
    std::vector<int> result;
    if (!m) {
        result.resize(n + 1);
        std::iota(result.begin(), result.end(), 0);
        return result;
    }
    auto pi = prefix_function(pattern);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j && text[i] != pattern[j]) j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == m) { result.push_back(i - m + 1); j = pi[j - 1]; }
    }
    return result;
}

// z[i] = LCP(s, s.substr(i)), with z[0] = 0 by convention.
// TC O(n), SC O(1) excluding output. [l,r) is the rightmost known Z-box.
inline std::vector<int> z_function(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}

class Trie {
    struct Node {
        std::array<int, 26> next;
        int through = 0, terminal = 0;
        Node() { next.fill(-1); }
    };
    std::vector<Node> nodes{1};
    static int letter(char ch) {
        assert(ch >= 'a' && ch <= 'z');
        return ch - 'a';
    }
    int walk(const std::string& s) const {
        int v = 0;
        for (char ch : s) {
            v = nodes[v].next[letter(ch)];
            if (v == -1) return -1;
        }
        return v;
    }
public:
    // Lowercase a-z only; duplicates and empty strings supported.
    // Insert amortized TC O(L); count/prefix_count worst-case TC O(L).
    // SC O(1) query, O(L) new nodes per insert.
    // Total storage O(26*S), S = total inserted characters + 1 (worst case).
    // Returns the terminal node ID, useful for associating payloads with words.
    int insert(const std::string& word) {
        int v = 0;
        ++nodes[v].through;
        for (char ch : word) {
            int c = letter(ch);
            if (nodes[v].next[c] == -1) {
                int child = static_cast<int>(nodes.size());
                nodes[v].next[c] = child;
                nodes.emplace_back();
            }
            v = nodes[v].next[c];
            ++nodes[v].through;
        }
        ++nodes[v].terminal;
        return v;
    }
    int count(const std::string& word) const {
        int v = walk(word);
        return v == -1 ? 0 : nodes[v].terminal;
    }
    int prefix_count(const std::string& prefix) const {
        int v = walk(prefix);
        return v == -1 ? 0 : nodes[v].through;
    }
    // O(1) traversal primitives for trie-guided DP/backtracking; root is node 0.
    // A missing transition returns -1; do not pass -1 back as a node.
    int transition(int node, char ch) const {
        assert(0 <= node && node < static_cast<int>(nodes.size()));
        return nodes[node].next[letter(ch)];
    }
    int terminal_count(int node) const {
        assert(0 <= node && node < static_cast<int>(nodes.size()));
        return nodes[node].terminal;
    }
};

class RollingHash {
    static constexpr std::array<i64, 2> mod{{1'000'000'007, 1'000'000'009}};
    static constexpr i64 base = 911382323;
    int n;
    std::vector<std::array<i64, 2>> prefix, power;
public:
    // Build TC/space O(n); substring hash TC/SC O(1).
    // Note 1: Hash equality is NOT proof of string equality. These fixed double
    // hashes can collide, especially with adversarial inputs. Prefer KMP/Z for
    // exact matching, or verify equal-hash candidates (which adds comparison cost).
    explicit RollingHash(const std::string& s)
        : n(static_cast<int>(s.size())), prefix(n + 1), power(n + 1) {
        power[0] = {1, 1};
        for (int i = 0; i < n; ++i) for (int k = 0; k < 2; ++k) {
            power[i + 1][k] = power[i][k] * base % mod[k];
            prefix[i + 1][k] = (prefix[i][k] * base + static_cast<unsigned char>(s[i]) + 1) % mod[k];
        }
    }
    std::array<i64, 2> hash(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        std::array<i64, 2> answer{};
        for (int k = 0; k < 2; ++k)
            answer[k] = (prefix[r][k] - prefix[l][k] * power[r - l][k] % mod[k] + mod[k]) % mod[k];
        return answer;
    }
};

struct PalindromeRadii {
    // odd[i]: radius INCLUDING center i. even[i]: radius centered between i-1,i.
    // Sum of all odd/even radii = number of nonempty palindromic substrings.
    std::vector<int> odd, even;
};
// Manacher: TC O(n), SC O(1) excluding O(n) output.
// Note 2: Mirrored radii reuse known comparisons. Successful new comparisons
// outside the current palindrome advance its right boundary at most n times.
inline PalindromeRadii manacher(const std::string& s) {
    int n = static_cast<int>(s.size());
    PalindromeRadii result{std::vector<int>(n), std::vector<int>(n)};
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 1 : std::min(result.odd[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) ++k;
        result.odd[i] = k--;
        if (i + k > r) { l = i - k; r = i + k; }
    }
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = i > r ? 0 : std::min(result.even[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) ++k;
        result.even[i] = k--;
        if (i + k > r) { l = i - k - 1; r = i + k; }
    }
    return result;
}
// pal[l][r] says s[l..r] (INCLUSIVE endpoints) is a palindrome.
// TC O(n^2), SC O(1) excluding O(n^2) output. Empty ranges are not stored.
// Shared by partition enumeration and minimum-cut DP.
inline std::vector<std::vector<char>> palindrome_table(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<std::vector<char>> pal(n, std::vector<char>(n));
    for (int l = n - 1; l >= 0; --l)
        for (int r = l; r < n; ++r)
            pal[l][r] = s[l] == s[r] && (r - l < 2 || pal[l + 1][r - 1]);
    return pal;
}
} // namespace cp
