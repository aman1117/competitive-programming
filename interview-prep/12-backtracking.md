# Backtracking & Recursion: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Backtracking and meet in the middle](../docs/backtracking/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 9 ⭐ problems first.

**Recognise it when:** "all", "every", "generate", or a constraint puzzle with small `n` (≤ ~15–20).

```text
void dfs(state):
    if done: record; return
    for choice in choices(state):
        if !valid(choice): continue
        apply(choice); dfs(next); undo(choice)
```

1. Decide the **decision tree**: include/exclude (subsets) vs. choose next element (permutations) vs. choose where the next piece ends (partitioning).
2. **Deduplicate** by sorting and skipping `nums[i] == nums[i−1]` at the same depth.
3. **Prune early** (remaining sum, counts, constraint sets). Interviewers grade pruning.
4. State the complexity honestly: `O(2^n · n)` for subsets, `O(n! · n)` for permutations.
5. If states repeat, add memoization → it's DP (Word Break II, Palindrome Partitioning).

---

## 1. Subsets & Combinations

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 78 | [Subsets](https://leetcode.com/problems/subsets/) ⭐ | Medium | include / exclude | `G` `U` 🔥 |
| ☐ | 90 | [Subsets II](https://leetcode.com/problems/subsets-ii/) | Medium | sort + skip same depth | `G` |
| ☐ | 77 | [Combinations](https://leetcode.com/problems/combinations/) | Medium | choose next ≥ start | `G` 🔥 |
| ☐ | 39 | [Combination Sum](https://leetcode.com/problems/combination-sum/) ⭐ | Medium | reuse allowed → stay at i | `G` `U` 🔥 |
| ☐ | 40 | [Combination Sum II](https://leetcode.com/problems/combination-sum-ii/) | Medium | no reuse, skip duplicates | `G` 🔥 |
| ☐ | 17 | [Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number/) ⭐ | Medium | product of letter choices | `G` `U` 🔥 |

## 2. Permutations

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 46 | [Permutations](https://leetcode.com/problems/permutations/) ⭐ | Medium | used[] or swapping | `G` `U` |
| ☐ | 47 | [Permutations II](https://leetcode.com/problems/permutations-ii/) | Medium | sort + skip used duplicates | `G` |
| ☐ | 60 | [Permutation Sequence](https://leetcode.com/problems/permutation-sequence/) | Hard | factorial number system (no backtracking) | `G` |

## 3. Partitioning & Generation

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 22 | [Generate Parentheses](https://leetcode.com/problems/generate-parentheses/) ⭐ | Medium | open ≤ n, close ≤ open | `G` `U` 🔥 |
| ☐ | 131 | [Palindrome Partitioning](https://leetcode.com/problems/palindrome-partitioning/) ⭐ | Medium | choose end of first palindrome | `G` 🔥 |
| ☐ | 93 | [Restore IP Addresses](https://leetcode.com/problems/restore-ip-addresses/) | Medium | 4 parts, validate each | `G` |
| ☐ | 282 | [Expression Add Operators](https://leetcode.com/problems/expression-add-operators/) ⭐ | Hard | track last operand for `*` | `G` |
| ☐ | 140 | [Word Break II](https://leetcode.com/problems/word-break-ii/) | Hard | memoize suffix results | `G` `U` |
| ☐ | 1096 | [Brace Expansion II](https://leetcode.com/problems/brace-expansion-ii/) | Hard | recursive grammar / stack | `G` |

## 4. Grid & Board Search

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 79 | [Word Search](https://leetcode.com/problems/word-search/) ⭐ | Medium | DFS + mark visited in place | `G` `U` 🔥 |
| ☐ | 51 | [N-Queens](https://leetcode.com/problems/n-queens/) ⭐ | Hard | cols / diagonals sets | `G` 🔥 |
| ☐ | 37 | [Sudoku Solver](https://leetcode.com/problems/sudoku-solver/) | Hard | row/col/box bitmasks | `G` `U` 🔥 |
| ☐ | 489 | [Robot Room Cleaner](https://leetcode.com/problems/robot-room-cleaner/) 🔒 | Hard | DFS with explicit move-back + relative directions | `G` |

## 5. Search with Arithmetic

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 679 | [24 Game](https://leetcode.com/problems/24-game/) | Hard | pick 2 numbers, combine, recurse | `G` `U` |
| ☐ | 465 | [Optimal Account Balancing](https://leetcode.com/problems/optimal-account-balancing/) 🔒 | Hard | net balances, settle first non-zero | `G` `U` |
| ☐ | 254 | [Factor Combinations](https://leetcode.com/problems/factor-combinations/) 🔒 | Medium | divisors ≥ previous factor | `U` |
| ☐ | 291 | [Word Pattern II](https://leetcode.com/problems/word-pattern-ii/) 🔒 | Medium | map pattern char → substring | `U` |

## 6. BFS + Backtracking

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 126 | [Word Ladder II](https://leetcode.com/problems/word-ladder-ii/) | Hard | BFS layers then DFS paths | `G` `U` |

---

**Interview-ready when:** you can draw the recursion tree, explain the pruning, and state the exponential complexity for each problem before coding.
