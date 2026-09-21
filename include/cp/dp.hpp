#pragma once
#include "strings.hpp"

namespace cp {
/// Indices of one STRICTLY increasing subsequence of maximum length.
/// TC O(n log n), SC O(n) tails + predecessor arrays, excluding output.
/// Change lower_bound to upper_bound for a nondecreasing subsequence.
inline std::vector<int> lis_indices(const std::vector<i64>& a) {
    std::vector<i64> tails;
    std::vector<int> last, parent(a.size(), -1);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        int p = static_cast<int>(std::lower_bound(tails.begin(), tails.end(), a[i]) - tails.begin());
        if (p) parent[i] = last[p - 1];
        if (p == static_cast<int>(tails.size())) { tails.push_back(a[i]); last.push_back(i); }
        else { tails[p] = a[i]; last[p] = i; }
    }
    std::vector<int> result;
    for (int v = last.empty() ? -1 : last.back(); v != -1; v = parent[v]) result.push_back(v);
    std::reverse(result.begin(), result.end());
    return result;
}

/// 0/1 knapsack; positive weights; each item at most once; capacity AT MOST W.
/// Selecting nothing allowed (negative values harmless). TC O(nW), SC O(W).
inline i64 knapsack_01(const std::vector<int>& weight, const std::vector<i64>& value, int capacity) {
    assert(weight.size() == value.size() && capacity >= 0);
    std::vector<i64> dp(capacity + 1);
    for (int i = 0; i < static_cast<int>(weight.size()); ++i) {
        assert(weight[i] > 0);
        // Note 1: Descending capacity reads states from BEFORE this item.
        // Ascending capacity would allow reusing it (unbounded knapsack).
        for (int c = capacity; c >= weight[i]; --c)
            dp[c] = std::max(dp[c], dp[c - weight[i]] + value[i]);
    }
    return dp[capacity];
}

/// Unlimited copies; strictly positive weights. TC O(nW), SC O(W).
inline i64 knapsack_unbounded(const std::vector<int>& weight, const std::vector<i64>& value, int capacity) {
    assert(weight.size() == value.size() && capacity >= 0);
    std::vector<i64> dp(capacity + 1);
    for (int i = 0; i < static_cast<int>(weight.size()); ++i) {
        assert(weight[i] > 0);
        for (int c = weight[i]; c <= capacity; ++c)
            dp[c] = std::max(dp[c], dp[c - weight[i]] + value[i]);
    }
    return dp[capacity];
}

/// Nonnegative elements, each at most once; exact target (empty subset allowed).
/// TC O(n*target), SC O(target). Negative elements need a different state range.
inline bool subset_sum(const std::vector<int>& a, int target) {
    assert(target >= 0);
    std::vector<char> dp(target + 1);
    dp[0] = true;
    for (int x : a) {
        assert(x >= 0);
        for (int s = target; s >= x; --s) dp[s] = dp[s] || dp[s - x];
    }
    return dp[target];
}

/// Minimum unlimited coins, exact amount; positive denominations. -1 if impossible.
/// TC O(number_of_coins * amount), SC O(amount).
inline int min_coins(const std::vector<int>& coins, int amount) {
    assert(amount >= 0 && amount < std::numeric_limits<int>::max());
    std::vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int coin : coins) {
        assert(coin > 0);
        for (int s = coin; s <= amount; ++s)
            if (dp[s - coin] != amount + 1) dp[s] = std::min(dp[s], dp[s - coin] + 1);
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

/// Number of unordered unlimited coin combinations modulo mod.
/// Denominations must be positive AND distinct (duplicates overcount types).
/// TC O(number_of_coins * amount), SC O(amount).
inline i64 coin_combinations(const std::vector<int>& coins, int amount, int mod = MOD) {
    assert(amount >= 0 && mod >= 1);
    std::vector<i64> dp(amount + 1);
    dp[0] = 1 % mod;
    for (int coin : coins) {
        assert(coin > 0);
        for (int s = coin; s <= amount; ++s) dp[s] = (dp[s] + dp[s - coin]) % mod;
    }
    return dp[amount];
}

/// LCS length only. TC O(nm+n+m), SC O(min(n,m)+1), including empty strings.
/// For positive lengths these are O(nm) / O(min(n,m)); saves the old diagonal.
/// To reconstruct an LCS simply, retain an O(nm) table and backtrack.
inline int lcs_length(const std::string& a, const std::string& b) {
    const std::string& outer = a.size() >= b.size() ? a : b;
    const std::string& inner = a.size() >= b.size() ? b : a;
    std::vector<int> dp(inner.size() + 1);
    for (char x : outer) {
        int diagonal = 0;
        for (int j = 1; j <= static_cast<int>(inner.size()); ++j) {
            int old = dp[j];
            dp[j] = x == inner[j - 1] ? diagonal + 1 : std::max(dp[j], dp[j - 1]);
            diagonal = old;
        }
    }
    return dp.back();
}

/// Levenshtein distance: insert/delete/replace each costs 1.
/// TC O(nm+n+m), SC O(min(n,m)+1), including empty strings.
inline int edit_distance(const std::string& a, const std::string& b) {
    const std::string& outer = a.size() >= b.size() ? a : b;
    const std::string& inner = a.size() >= b.size() ? b : a;
    std::vector<int> dp(inner.size() + 1);
    std::iota(dp.begin(), dp.end(), 0);
    for (int i = 1; i <= static_cast<int>(outer.size()); ++i) {
        int diagonal = dp[0];
        dp[0] = i;
        for (int j = 1; j <= static_cast<int>(inner.size()); ++j) {
            int old = dp[j];
            dp[j] = outer[i - 1] == inner[j - 1]
                ? diagonal : 1 + std::min({diagonal, dp[j], dp[j - 1]});
            diagonal = old;
        }
    }
    return dp.back();
}

struct Job { i64 start, end, profit; };
/// Weighted interval scheduling, [start,end), start < end. Empty schedule allowed.
/// TC O(n log n), SC O(n), including sorting copy and DP.
inline i64 weighted_scheduling(std::vector<Job> jobs) {
    for (auto j : jobs) assert(j.start < j.end);
    std::sort(jobs.begin(), jobs.end(), [](auto a, auto b) { return a.end < b.end; });
    int n = static_cast<int>(jobs.size());
    std::vector<i64> ends, dp(n + 1);
    for (auto j : jobs) ends.push_back(j.end);
    for (int i = 0; i < n; ++i) {
        int compatible = static_cast<int>(std::upper_bound(ends.begin(), ends.begin() + i, jobs[i].start) - ends.begin());
        dp[i + 1] = std::max(dp[i], dp[compatible] + jobs[i].profit);
    }
    return dp[n];
}

/// Matrix-chain multiplication: matrix i has dimensions d[i] x d[i+1].
/// TC O(n^3), SC O(n^2); O(n^2) intervals each try O(n) split points.
/// Every scalar product and total cost must fit below INF.
inline i64 matrix_chain(const std::vector<i64>& dimensions) {
    assert(dimensions.size() >= 2);
    for (i64 d : dimensions) assert(d > 0);
    int n = static_cast<int>(dimensions.size()) - 1;
    std::vector<std::vector<i64>> dp(n, std::vector<i64>(n));
    for (int length = 2; length <= n; ++length)
        for (int l = 0; l + length <= n; ++l) {
            int r = l + length - 1;
            dp[l][r] = INF;
            for (int k = l; k < r; ++k)
                dp[l][r] = std::min(dp[l][r], dp[l][k] + dp[k + 1][r]
                    + dimensions[l] * dimensions[k + 1] * dimensions[r + 1]);
        }
    return dp[0][n - 1];
}

/// Held-Karp TSP: minimum Hamiltonian cycle from 0, returning to 0.
/// Square cost matrix, INF for missing edges. n<=20 (20 uses about 160 MiB).
/// TC O(n^2 * 2^n), SC O(n * 2^n); nullopt if no tour; n<=1 -> cost 0.
inline std::optional<i64> traveling_salesman(const std::vector<std::vector<i64>>& cost) {
    int n = static_cast<int>(cost.size());
    assert(n <= 20);
    for (const auto& row : cost) assert(static_cast<int>(row.size()) == n);
    if (n <= 1) return 0;
    int states = 1 << n;
    std::vector<i64> dp(static_cast<std::size_t>(states) * n, INF);
    auto at = [&](int mask, int v) -> i64& { return dp[static_cast<std::size_t>(mask) * n + v]; };
    at(1, 0) = 0;
    for (int mask = 1; mask < states; mask += 2)
        for (int v = 0; v < n; ++v) if ((mask & (1 << v)) && at(mask, v) != INF)
            for (int u = 0; u < n; ++u) if (!(mask & (1 << u)) && cost[v][u] != INF)
                at(mask | (1 << u), u) = std::min(at(mask | (1 << u), u), at(mask, v) + cost[v][u]);
    i64 answer = INF;
    for (int v = 1; v < n; ++v) if (at(states - 1, v) != INF && cost[v][0] != INF)
        answer = std::min(answer, at(states - 1, v) + cost[v][0]);
    return answer == INF ? std::nullopt : std::optional<i64>{answer};
}

/// Digit DP: count x in [0,bound] whose decimal digits sum to target.
/// Includes zero when target==0. Negative bound -> 0 for range-query convenience.
/// bound <= 10^18. TC O(D * (target+1) * 10), SC O(target+1), D=digits.
inline i64 count_digit_sum(i64 bound, int target) {
    assert(target >= 0 && bound <= 1'000'000'000'000'000'000LL);
    if (bound < 0) return 0;
    std::string digits = std::to_string(bound);
    if (target > 9 * static_cast<int>(digits.size())) return 0;
    std::vector<std::array<i64, 2>> dp(target + 1);
    dp[0][1] = 1;
    for (char ch : digits) {
        std::vector<std::array<i64, 2>> next(target + 1);
        for (int sum = 0; sum <= target; ++sum) for (int tight = 0; tight < 2; ++tight) {
            int limit = tight ? ch - '0' : 9;
            for (int digit = 0; digit <= limit && sum + digit <= target; ++digit)
                next[sum + digit][tight && digit == limit] += dp[sum][tight];
        }
        dp.swap(next);
    }
    // Note 2: Leading zeros do not change a digit sum. Constraints about actual
    // digits (e.g. no repeated digits) also need a "started" state.
    return dp[target][0] + dp[target][1];
}

/// SOS DP: output[mask] = sum of input[sub] over all submasks.
/// Input length MUST be 2^B, B>=0. TC O(B*2^B), SC O(1) beyond by-value O(2^B)
/// working/result vector. Direct all-mask/all-submask enumeration is O(3^B).
inline std::vector<i64> subset_zeta(std::vector<i64> values) {
    std::size_t n = values.size();
    assert(n && (n & (n - 1)) == 0);
    for (std::size_t bit = 1; bit < n; bit <<= 1)
        for (std::size_t mask = 0; mask < n; ++mask)
            if (mask & bit) values[mask] += values[mask ^ bit];
    return values;
}
/// House Robber I/II: maximum nonadjacent sum, empty choice allowed.
/// circular=true also forbids taking both endpoints (except for a singleton).
/// TC O(n), SC O(1). Circular optimum omits either the first or the last item.
inline i64 nonadjacent_sum(const std::vector<i64>& a, bool circular = false) {
    int n = static_cast<int>(a.size());
    if (!n) return 0;
    if (n == 1) return std::max(0LL, a[0]);
    auto linear = [&](int l, int r) {
        i64 previous = 0, two_back = 0;
        for (int i = l; i < r; ++i) {
            i64 current = std::max(previous, two_back + a[i]);
            two_back = previous; previous = current;
        }
        return previous;
    };
    return circular ? std::max(linear(0, n - 1), linear(1, n)) : linear(0, n);
}

/// Stock I/II/III/IV + transaction fee: at most k completed buy/sell pairs,
/// one share at a time, fee charged at SELL, nonnegative prices/fee, no cooldown.
/// TC O(n*(k+1)), SC O(k+1); if k>=n/2, unlimited fast path is O(n)/O(1).
inline i64 stock_profit_k(const std::vector<i64>& prices, int k, i64 fee = 0) {
    assert(k >= 0 && fee >= 0);
    for (i64 p : prices) { assert(p >= 0); (void)p; }
    int n = static_cast<int>(prices.size());
    if (n < 2 || k == 0) return 0;
    if (k >= n / 2) {
        i64 cash = 0, hold = -prices[0];
        for (int i = 1; i < n; ++i) {
            i64 previous_cash = cash;
            cash = std::max(cash, hold + prices[i] - fee);
            hold = std::max(hold, previous_cash - prices[i]);
        }
        return cash;
    }
    std::vector<i64> cash(k + 1), hold(k + 1, -INF);
    for (i64 price : prices) {
        // Descending transaction budget preserves yesterday's cash[t-1].
        for (int t = k; t >= 1; --t) {
            if (hold[t] != -INF) cash[t] = std::max(cash[t], hold[t] + price - fee);
            hold[t] = std::max(hold[t], cash[t - 1] - price);
        }
    }
    return cash[k];
}

/// Unlimited trades with ONE-DAY cooldown after each sale, plus optional fee.
/// TC O(n), SC O(1). free/hold/sold are yesterday's mutually exclusive states.
inline i64 stock_profit_cooldown(const std::vector<i64>& prices, i64 fee = 0) {
    assert(fee >= 0);
    i64 free = 0, hold = -INF, sold = -INF;
    for (i64 price : prices) {
        assert(price >= 0);
        i64 next_hold = std::max(hold, free - price);
        i64 next_sold = hold == -INF ? -INF : hold + price - fee;
        free = std::max(free, sold);
        hold = next_hold; sold = next_sold;
    }
    return std::max(free, sold);
}

/// Lowercase a-z text/dictionary. Reusable dictionary words; empty words do not
/// advance the segmentation. Empty text is segmentable.
/// TC O(w+S+n*(L+1)), SC O(26S+n), w=word count, S=total dictionary characters,
/// L=max word length (0 for an empty dictionary).
/// Trie traversal avoids repeatedly allocating/hashing substrings (hidden O(L)).
inline bool word_break(const std::string& text, const std::vector<std::string>& dictionary) {
    Trie trie;
    for (const auto& word : dictionary) trie.insert(word);
    int n = static_cast<int>(text.size());
    std::vector<char> reachable(n + 1);
    reachable[0] = true;
    for (int l = 0; l < n; ++l) if (reachable[l]) {
        int node = 0;
        for (int r = l; r < n; ++r) {
            node = trie.transition(node, text[r]);
            if (node == -1) break;
            if (trie.terminal_count(node)) reachable[r + 1] = true;
        }
    }
    return reachable[n];
}

/// Number of ways target is a subsequence of source, modulo mod.
/// For exact counting, adapt the addition ONLY if every intermediate fits i64.
/// TC O(nm+n+m), SC O(m+1); reverse j to avoid reusing one source character.
inline i64 distinct_subsequences(const std::string& source, const std::string& target, int mod = MOD) {
    assert(mod > 0);
    int m = static_cast<int>(target.size());
    std::vector<i64> dp(m + 1);
    dp[0] = 1 % mod;
    for (char ch : source)
        for (int j = m; j > 0; --j)
            if (ch == target[j - 1]) dp[j] = (dp[j] + dp[j - 1]) % mod;
    return dp[m];
}

/// Minimum cuts into nonempty palindromic pieces; empty string -> 0.
/// TC/SC O(n^2) using the shared palindrome table and O(n) prefix DP.
inline int palindrome_min_cuts(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (!n) return 0;
    auto pal = palindrome_table(s);
    std::vector<int> pieces(n + 1, n + 1);
    pieces[0] = 0;
    for (int r = 1; r <= n; ++r)
        for (int l = 0; l < r; ++l) if (pal[l][r - 1])
            pieces[r] = std::min(pieces[r], pieces[l] + 1);
    return pieces[n] - 1;
}

/// Burst Balloons: nonnegative values, multiply current neighbors when removed.
/// TC O(n^3), SC O(n^2). Choose the LAST balloon in an open interval, so its
/// neighbors are fixed interval boundaries and the two subproblems are independent.
inline i64 burst_balloons(const std::vector<i64>& values) {
    int n = static_cast<int>(values.size());
    std::vector<i64> a{1};
    for (i64 x : values) { assert(x >= 0); a.push_back(x); }
    a.push_back(1);
    std::vector<std::vector<i64>> dp(n + 2, std::vector<i64>(n + 2));
    for (int length = 2; length < n + 2; ++length)
        for (int l = 0; l + length < n + 2; ++l) {
            int r = l + length;
            for (int last = l + 1; last < r; ++last)
                dp[l][r] = std::max(dp[l][r], dp[l][last] + dp[last][r] + a[l] * a[last] * a[r]);
        }
    return dp[0][n + 1];
}

/// Optimal play taking one value from either end each turn: first player's
/// final score minus second player's score. Negative values allowed.
/// TC O(n^2), SC O(n); zero for empty input. dp[l,r] = max(a[l]-dp[l+1,r],
/// a[r]-dp[l,r-1]); subtraction swaps the current player's perspective.
inline i64 take_ends_score_difference(const std::vector<i64>& a) {
    int n = static_cast<int>(a.size());
    if (!n) return 0;
    std::vector<i64> dp(n);
    for (int l = n - 1; l >= 0; --l) {
        dp[l] = a[l];
        for (int r = l + 1; r < n; ++r)
            dp[r] = std::max(a[l] - dp[r], a[r] - dp[r - 1]);
    }
    return dp[n - 1];
}

/// Digit DP: count POSITIVE x<=bound whose digits are all distinct; bound<=10^18.
/// mask=0 means "not started", so leading zero padding never consumes digit 0.
/// TC O(D*2^10*10), SC O(2^10), including two tight states and rolling layers.
inline i64 count_unique_digit_numbers(i64 bound) {
    assert(bound <= 1'000'000'000'000'000'000LL);
    if (bound <= 0) return 0;
    std::array<std::array<i64, 2>, 1024> dp{};
    dp[0][1] = 1;
    for (char ch : std::to_string(bound)) {
        std::array<std::array<i64, 2>, 1024> next{};
        for (int mask = 0; mask < 1024; ++mask) for (int tight = 0; tight < 2; ++tight) {
            int limit = tight ? ch - '0' : 9;
            for (int digit = 0; digit <= limit; ++digit) {
                bool leading = mask == 0 && digit == 0;
                if (!leading && (mask & (1 << digit))) continue;
                int next_mask = leading ? 0 : mask | (1 << digit);
                next[next_mask][tight && digit == limit] += dp[mask][tight];
            }
        }
        dp = next;
    }
    i64 answer = 0;
    for (int mask = 1; mask < 1024; ++mask) answer += dp[mask][0] + dp[mask][1];
    return answer;
}
/// Wildcard full-string match: '?' consumes one byte, '*' any number of bytes.
/// This is NOT regex: "a*" means literal 'a' followed by an arbitrary suffix.
/// TC O(nm+n+m), SC O(m+1), n=text length, m=pattern length.
inline bool wildcard_match(const std::string& text, const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<char> dp(m + 1);
    dp[0] = true;
    for (int j = 1; j <= m; ++j) dp[j] = dp[j - 1] && pattern[j - 1] == '*';
    for (char ch : text) {
        bool diagonal = dp[0];
        dp[0] = false;
        for (int j = 1; j <= m; ++j) {
            bool old = dp[j];
            if (pattern[j - 1] == '*') dp[j] = dp[j - 1] || dp[j];
            else dp[j] = diagonal && (pattern[j - 1] == '?' || pattern[j - 1] == ch);
            diagonal = old;
        }
    }
    return dp[m];
}
} // namespace cp
