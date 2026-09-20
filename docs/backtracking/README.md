# Backtracking, divide and conquer, and meet in the middle

[Handbook](../README.md) | [Implementation](../../include/cp/search.hpp)

**Prerequisites:** recursion, mutable state, and subset masks.  
**Goal:** distinguish enumeration, decision, and counting; explain why pruning helps without disguising exponential worst cases.

## 1. The choose-explore-undo contract

A backtracking state describes one partial solution. Make a choice, recurse, then restore every changed component before exploring the next choice. The restoration is part of correctness, not merely cleanup.

Callback-based enumerators reuse their working path. Copy outputs you retain and never modify or keep a reference to the temporary path. Space bounds that exclude output do not include your callback's accumulated results.

## 2. Subsets, permutations, and submasks

`enumerate_subsets` chooses increasing indices, so each subset of **positions** appears once. Equal values at different positions remain different choices.

`enumerate_permutations` sorts first and repeatedly uses `next_permutation`, generating unique value permutations even with duplicates.

```cpp
int subsets = 0;
cp::enumerate_subsets({1, 2}, [&](const auto&) { ++subsets; });
assert(subsets == 4); // {}, {1}, {1,2}, {2}.

std::vector<std::vector<cp::i64>> permutations;
cp::enumerate_permutations({1, 1, 2}, [&](const auto& p) { permutations.push_back(p); });
assert(permutations.size() == 3);

std::vector<std::uint64_t> masks;
cp::enumerate_submasks(0b1010, [&](auto sub) { masks.push_back(sub); });
assert((masks == std::vector<std::uint64_t>{10, 8, 2, 0}));
```

For submasks, repeatedly use `(sub-1)&mask`. Zero must be handled before subtracting again, or unsigned wraparound returns to the original mask.

**TC/SC:** subset traversal costs `O(2^n + callback work)` time and `O(n)` stack/path space. Copying all subsets adds `O(n*2^n)` output work. Unique permutations cost `O(n log n+nP+callback work)`, `P<=n!`, with an `O(n)` input copy. Submask enumeration makes `2^popcount(mask)` callback visits with `O(1)` own workspace.

## 3. Combination sum and duplicate control

**Use when:** positive candidate values must sum to an exact target.

Sort candidates. The start index prevents permutations of the same combination. For unlimited reuse, recurse with the same candidate index; for one use per occurrence, recurse with the next index.

Skip equal candidates at the **same recursion depth**. In the one-use variant, a later equal occurrence may still be used deeper in the path.

```cpp
auto unlimited = cp::combination_sum({2, 3, 6, 7}, 7);
assert((unlimited == std::vector<std::vector<cp::i64>>{{2, 2, 3}, {7}}));
auto once = cp::combination_sum({10, 1, 2, 7, 6, 1, 5}, 8, false);
assert((once == std::vector<std::vector<cp::i64>>{{1, 1, 6}, {1, 2, 5}, {1, 7}, {2, 6}}));
```

Choosing the first `1` may be followed by the second `1`, enabling `[1,1,6]`. But starting two sibling branches with those equal ones would duplicate answers.

**TC/SC:** one-use enumeration has an `O(n*2^n + output_size)` upper bound. For unlimited reuse with maximum path length `h<=target/min_value`, a conservative bound is `O(n log n+n*C(n+h,h)+output_size)`: at most that many nondecreasing partial combinations, scanning up to `n` choices each. Space is `O(n+h)` excluding output.

This is not `O(n*target)` coin DP: it emits actual combinations. Candidates must be positive; zero can produce nonterminating unlimited recursion. Very large targets can overflow the native recursion stack.

## 4. Balanced parentheses

**Use when:** valid prefixes can be recognized before complete candidates are built.

Track opened and closed counts. Add `(` only while fewer than `n` pairs have been opened; add `)` only when an unmatched opening exists. This avoids exploring prefixes that can never become valid.

For three pairs, valid outputs include `"((()))"` and `"()(())"`; a prefix starting with `")"` is rejected immediately.

```cpp
std::vector<std::string> result;
cp::enumerate_parentheses(3, [&](const std::string& s) { result.push_back(s); });
assert(result.size() == 5);
```

**TC/SC:** `O(n*C_n + callback work)` time and `O(n)` working space, where `C_n` is the Catalan number of outputs. The output count itself is exponential, so pruning cannot make complete enumeration polynomial.

## 5. Palindrome partitions

**Use when:** every valid partition is required, not merely the minimum number of cuts.

Precompute a [palindrome table](../strings/README.md), then choose each palindromic prefix of the remaining suffix. Every chosen endpoint is a cut; continue until the entire string is covered.

```cpp
std::vector<std::vector<std::string>> partitions;
cp::enumerate_palindrome_partitions("aab", [&](const auto& path) { partitions.push_back(path); });
assert((partitions == std::vector<std::vector<std::string>>{{"a", "a", "b"}, {"aa", "b"}}));
```

**TC/SC:** `O(n^2+n*2^n+callback work)` time, `O(n^2+n)` working space. A string with all equal characters makes every cut pattern valid. Copying partition strings and retained outputs must be counted. An empty string emits one empty partition.

## 6. Single-word grid search

**Use when:** a word must follow neighboring cells without reusing a cell.

The state includes the current cell, character position, and cells already used on this path. Mark a cell before exploring neighbors and unmark it before returning, even after success.

```cpp
std::vector<std::string> board{"ABCE", "SFCS", "ADEE"};
assert(cp::word_exists(board, "ABCCED"));
assert(!cp::word_exists(board, "ABCB"));
assert(board[0] == "ABCE"); // Input is unchanged.
```

`"ABCCED"` follows cells `(0,0),(0,1),(0,2),(1,2),(2,2),(2,1)`. `"ABCB"` would need to reuse the earlier `B`.

**TC/SC:** for word length `L>0`, a conservative time bound is `O(RC*4*3^(L-1)+R)`. After the first move, the previous cell is unavailable, leaving at most three new directions. Space is `O(RC+L)` for the used-cell matrix and recursion. Empty word returns true.

## 7. Multiword search with trie pruning

**Use when:** many dictionary words must be found on the same board.

Instead of searching each word independently, carry a trie node along each board path. If a transition is missing, no dictionary word extends that prefix, so stop immediately. A terminal node identifies a found word; clear its output payload after emitting it to prevent duplicates.

```cpp
auto found = cp::find_words({"oaan", "etae", "ihkr", "iflv"},
                            {"oath", "pea", "eat", "rain", "eat"});
std::sort(found.begin(), found.end());
assert((found == std::vector<std::string>{"eat", "oath"}));
```

The repeated `"eat"` dictionary entry still produces one output. The board is not modified.

**TC/SC:** `O(S+w+R+RC*4*3^(L-1)+output_characters)` time and `O(26S+RC+L)` workspace, for total dictionary characters `S`, word count `w`, and maximum word length `L>=1`. Trie pruning helps enormously on many inputs but does not eliminate exponential worst cases. Board/dictionary must be lowercase `a-z`; dictionary words are nonempty.

## 8. N-Queens with bitmasks

**Use when:** placing one queen per row, avoiding occupied columns and diagonals.

Maintain occupied-column and two diagonal masks. Available columns are the board mask minus those three attack masks. After placing a queen, shift diagonal attacks left/right for the next row.

For a four-by-four board, column choices `[1,3,0,2]` form one valid solution. The other is `[2,0,3,1]`.

```cpp
std::vector<std::vector<int>> boards;
cp::enumerate_n_queens(4, [&](const auto& columns) { boards.push_back(columns); });
assert((boards == std::vector<std::vector<int>>{{1, 3, 0, 2}, {2, 0, 3, 1}}));
```

**TC/SC:** the implementation documents a conservative `O(n*n! + callback work)` bound because extracting a bit's column index can take `O(n)` work. Stack/path space is `O(n)`. The mask supports up to 20, but roughly `n<=15` is the intended practical range. The number of stored board strings, if you generate them, adds separate output cost.

## 9. Meet in the middle

**Use when:** there are about 40 subset choices, too many for `2^n` brute force but manageable as two `2^(n/2)` lists.

Split the array and compute `subset_sums` for each half. For every left sum `x`, count right sums equal to `target-x` using binary search on the sorted right list.

For `[3,-1,2,5]` and target four, left sums from `[3,-1]` include `-1` and `2`; they pair with right sums `5` and `2`, yielding subsets `[-1,5]` and `[3,-1,2]`.

```cpp
assert((cp::subset_sums({3, -1}) == std::vector<cp::i64>{0, 3, -1, 2}));
assert(cp::count_subsets_sum({3, -1, 2, 5}, 4) == 2);
```

**TC/SC:** generating one half of size `h` costs `O(2^h)` time/output storage. The complete counting routine costs `O(n*2^ceil(n/2))` time and `O(2^ceil(n/2))` space. Duplicates represent separate subsets of positions; use equal-range counts, not just presence. Negative values are allowed, and the empty subset is included.

## 10. Merge-sort inversion counting

**Use when:** counting pairs `i<j` with `a[i]>a[j]`.

Recursively sort both halves. During merging, if the next right value is smaller than the next left value, it is smaller than every remaining left value; add that entire remaining count.

For `[3,1,2]`, merging left `[3]` with right `[1,2]` adds one inversion for each of `1` and `2`.

```cpp
std::vector<cp::i64> values{3, 1, 2};
assert(cp::count_inversions(values) == 2);
assert((values == std::vector<cp::i64>{1, 2, 3}));
```

**TC/SC:** `O(n log n)` time, `O(n)` merge buffer plus `O(log n)` recursion. Equal values are not inversions. The input is sorted as a side effect; copy it yourself if its original order is needed later.
