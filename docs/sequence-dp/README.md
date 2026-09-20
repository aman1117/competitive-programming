# Sequence and state-machine dynamic programming

[Handbook](../README.md) | [Implementation](../../include/cp/dp.hpp)

**Prerequisites:** arrays, recurrence relations, and basic state-space reasoning.  
**Goal:** define exactly what a state means before deciding how to optimize its storage.

## DP design checklist

State what information determines future choices. Define the base cases, unreachable states, transition, processing order, and final answer. Only then reduce memory. A correct recurrence with the wrong update order solves a different problem.

## 1. Longest increasing subsequence

**Use when:** finding a longest order-preserving subsequence with strictly increasing values.

Maintain `tails[len-1]`, the smallest possible final value of an increasing subsequence of length `len`. A smaller tail leaves at least as many opportunities for future extension. Replace the first tail not smaller than the new value using `lower_bound`.

For `[10,9,2,5,3,7,101,18]`, representative tails evolve toward `[2,3,7,18]`. Tails alone do not preserve predecessor relationships, so the implementation separately stores indices and parents for reconstruction.

```cpp
std::vector<cp::i64> values{10, 9, 2, 5, 3, 7, 101, 18};
auto indices = cp::lis_indices(values);
assert(indices.size() == 4);
for (std::size_t i = 1; i < indices.size(); ++i)
    assert(indices[i - 1] < indices[i] && values[indices[i - 1]] < values[indices[i]]);
```

**TC/SC:** `O(n log n)` time from one binary search per value, `O(n)` workspace for tails/parents, plus output. Use `upper_bound`, not `lower_bound`, for a nondecreasing variant. A subsequence need not be contiguous.

## 2. 0/1 and unbounded knapsack

**Use when:** each item consumes an integer resource and earns a value.

For at-most capacity `c`, either skip an item or take it after solving capacity `c-weight`. The difference between 0/1 and unlimited copies is **where the previous state comes from**.

| Variant | Capacity iteration | Why |
|---|---|---|
| 0/1 | Descending | `dp[c-weight]` still belongs to the previous item layer |
| Unbounded | Ascending | `dp[c-weight]` may already include this item |

```cpp
assert(cp::knapsack_01({2, 3, 4}, {4, 5, 7}, 5) == 9);
assert(cp::knapsack_unbounded({2, 3}, {3, 5}, 7) == 11);
```

The 0/1 answer takes weights two and three for value nine. The unlimited answer takes `2+2+3` for value `3+3+5=11`.

**TC/SC:** `O(nW)` time and `O(W)` space for positive dimensions. This is pseudo-polynomial: `W` is a numeric capacity, not its encoded bit length. Weights must be positive. These routines allow taking nothing and optimize **at most** capacity; exact-fill variants need explicit unreachable states.

## 3. Subset sum

**Use when:** deciding whether a nonnegative set of values can total an exact target.

`dp[s]` means some subset of processed values reaches `s`. Initialize only `dp[0]=true`. Descending sums enforce one use per input occurrence.

```cpp
assert(cp::subset_sum({1, 5, 11, 5}, 11));
assert(!cp::subset_sum({2, 4}, 3));
```

Target eleven can be formed by the single `11` or by `1+5+5`.

**TC/SC:** `O(nT)` time, `O(T)` space. Zero values are allowed; negative values need a different state range or another method. Equal values at different indices are separate available items.

## 4. Coin change: optimization versus counting

**Use when:** unlimited positive denominations form an exact amount.

For minimum coins, use `dp[s]=min(dp[s],dp[s-coin]+1)` from reachable states. Greedy largest-first is not generally correct: with `[1,3,4]`, amount six is best as `3+3`, not `4+1+1`.

For unordered combinations, put coins outside and sums ascending inside. Each combination is introduced when its largest allowed denomination is processed.

```cpp
assert(cp::min_coins({1, 3, 4}, 6) == 2);
assert(cp::min_coins({2}, 3) == -1);
assert(cp::coin_combinations({1, 2, 5}, 5) == 4);
```

The four combinations for five are `1+1+1+1+1`, `1+1+1+2`, `1+2+2`, and `5`.

**TC/SC:** `O(number_of_coins*amount)` time, `O(amount)` space. Counting requires distinct denominations; duplicates would count the same value as separate coin types. Reversing the loop roles can count ordered sequences instead. `coin_combinations` returns a modular count, not an arbitrary-precision exact count.

## 5. LCS and edit distance

Both compare prefixes of two sequences, but their state meanings differ.

**LCS:** if final characters match, extend the diagonal result; otherwise omit one final character and take the better result.

**Edit distance:** choose deletion, insertion, or replacement; matching characters have zero replacement cost.

```cpp
assert(cp::lcs_length("abcde", "ace") == 3);
assert(cp::edit_distance("horse", "ros") == 3);
```

The LCS is `"ace"`. One three-edit route is `horse -> rorse -> rose -> ros`.

The optimized implementation stores one row and saves the previous diagonal before overwriting it. Choosing the shorter string for columns reduces memory.

**TC/SC:** `O(nm+n+m)` time and `O(min(n,m)+1)` space, including empty strings. For nonempty inputs this is the familiar `O(nm)`/`O(min(n,m))`. LCS returns only a length; straightforward reconstruction usually retains an `O(nm)` table. Edit distance here uses unit insertion/deletion/replacement costs.

## 6. Counting distinct subsequence selections

**Use when:** counting ways to choose source positions that spell a target.

Let `dp[j]` count ways to form the first `j` target characters. When source character `x` matches target position `j-1`, add `dp[j-1]` to `dp[j]`. Process `j` backward so one source position cannot fill multiple target positions.

```cpp
assert(cp::distinct_subsequences("rabbbit", "rabbit") == 3);
assert(cp::distinct_subsequences("abc", "") == 1);
```

The three equal `b`s supply two target `b`s in three choices of positions. Equal resulting strings can still arise from different valid selections.

**TC/SC:** `O(nm+n+m)` time, `O(m+1)` space. The API counts modulo its modulus. Some LeetCode problems require exact counts; remove modular arithmetic only after proving every intermediate count fits the chosen integer type.

## 7. Word-break DP with a trie

**Use when:** deciding whether a string can be segmented into reusable dictionary words.

`reachable[i]` means the prefix ending at boundary `i` is segmentable. From each reachable boundary, walk the trie along the text. A terminal node creates another reachable boundary; a missing edge ends that search.

For `"leetcode"` and `{"leet","code"}`, boundary zero reaches four through `"leet"`, then four reaches eight through `"code"`.

```cpp
assert(cp::word_break("leetcode", {"leet", "code"}));
assert(!cp::word_break("catsandog", {"cats", "dog", "sand", "and", "cat"}));
assert(cp::word_break("", {}));
```

**TC/SC:** `O(w+S+n(L+1))` time and `O(26S+n)` space, where `w` is word count, `S` total dictionary characters, and `L` maximum word length. Trie walking avoids the hidden substring allocation/hashing costs of repeatedly constructing candidate strings. Text and dictionary are lowercase `a-z`; empty words do not advance a boundary.

## 8. Wildcard matching

**Use when:** matching an entire string against a pattern with `?` and `*`.

For `?` or a matching literal, use the previous diagonal state. For `*`, either consume no character (`dp[i][j-1]`) or consume one more text character while keeping the star (`dp[i-1][j]`).

```cpp
assert(cp::wildcard_match("adceb", "*a*b"));
assert(!cp::wildcard_match("acdcb", "a*c?b"));
assert(cp::wildcard_match("", "***"));
```

In the first example, the initial star is empty, `a` matches the first character, the second star consumes `"dce"`, and the final `b` matches.

**TC/SC:** `O(nm+n+m)` time, `O(m+1)` space. This is **not regex**: `a*` means literal `a` followed by any suffix, not zero or more repetitions of `a`.

## 9. House Robber: linear and circular

**Use when:** adjacent choices are incompatible.

For a prefix ending at `i`, either skip `i` and keep the previous optimum, or take it and add the optimum ending before its neighbor:

```text
best[i] = max(best[i-1], best[i-2] + value[i])
```

For `[2,7,9,3,1]`, prefix optima are `2,7,11,11,12`.

```cpp
assert(cp::nonadjacent_sum({2, 7, 9, 3, 1}) == 12);
assert(cp::nonadjacent_sum({2, 3, 2}, true) == 3);
```

In a circle, both endpoints cannot be chosen together. Solve once without the first and once without the last, taking the larger answer. A singleton is handled separately.

**TC/SC:** `O(n)` time and `O(1)` space. Empty selection is allowed, so negative values never force a negative answer.

## 10. Stock DP with transaction budgets and fees

**Use when:** choices depend on whether a stock is held and how many trades are available.

`cash[t]` is the best profit without a stock using at most `t` completed trades; `hold[t]` is the corresponding holding state with room to complete that trade. A sale pays the optional fee exactly once.

Update transaction budgets in descending order so buying from `cash[t-1]` reads the correct earlier state.

```cpp
assert(cp::stock_profit_k({3, 2, 6, 5, 0, 3}, 2) == 7);
assert(cp::stock_profit_k({1, 3, 2, 8, 4, 9}, 99, 2) == 8);
```

The first case buys at two and sells at six, then buys at zero and sells at three. With fees, repeatedly collecting every positive day-to-day difference is not valid because it may pay unnecessary fees.

**TC/SC:** `O(n(k+1))` time and `O(k+1)` space in the bounded case. If `k>=n/2`, the budget cannot bind; a two-state unlimited-trade path takes `O(n)` time and `O(1)` space. Prices and fees must be nonnegative. This API has no cooldown.

## 11. Stock cooldown

**Use when:** selling prevents buying on the following day.

Keep three previous-day states: free to buy, holding, and just sold. Buying can come only from the previous free state; previous sold can become free but cannot immediately buy.

For prices `[1,2,3,0,2]`:

| Price | Free | Hold | Just sold |
|---|---:|---:|---:|
| 1 | 0 | -1 | unreachable |
| 2 | 0 | -1 | 1 |
| 3 | 1 | -1 | 2 |
| 0 | 2 | 1 | -1 |
| 2 | 2 | 1 | 3 |

```cpp
assert(cp::stock_profit_cooldown({1, 2, 3, 0, 2}) == 3);
```

The optimum sells on day one, rests on day two, then buys at zero and sells at two. A holding state can be positive because earlier realized profit can finance the held stock.

**TC/SC:** `O(n)` time, `O(1)` space. Compute new states from old states; careless in-place ordering can accidentally remove the cooldown.

## 12. Weighted interval scheduling

**Use when:** nonoverlapping jobs have different profits, so earliest-finish greedy alone is insufficient.

Sort jobs by end time. For each job, binary-search how many earlier jobs end no later than its start. Either skip the current job or combine its profit with the best compatible prefix.

```cpp
assert(cp::weighted_scheduling({{1, 3, 50}, {2, 4, 10}, {3, 5, 40}, {4, 6, 70}}) == 120);
```

The prefix optima are `0,50,50,90,120`. The final job combines with the first for `50+70`, not necessarily with the immediately preceding job.

**TC/SC:** `O(n log n)` time for sorting and predecessor searches, `O(n)` workspace including the copied jobs and DP. Intervals are nonempty and half-open, so ending exactly when another starts is compatible. Empty schedule is allowed.
