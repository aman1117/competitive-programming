# Arrays, Hashing & Prefix Sums: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Search, prefixes, and windows](../docs/search-and-windows/README.md), [Monotonic structures and greedy §6, §9](../docs/monotonic-and-greedy/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 11 ⭐ problems first.

**Recognise it when:** you need *"have I seen X before?"*, a frequency count, a grouping key, or *"sum/count of a subarray"*.

1. **Hash map = memory of the past.** Scan once; at index `i`, ask the map about `0..i-1` (Two Sum, Subarray Sum = K).
2. **Prefix sum + hash map** counts subarrays: `sum(l..r) = P[r+1] − P[l]`, so look up `P[r+1] − k`. Seed the map with `{0: 1}`.
3. **Canonical key** for grouping: sorted string, character-count tuple, normalised slope `(dy/g, dx/g)`.
4. **In-place index tricks** (First Missing Positive, matrix rotation) turn O(n) extra space into O(1) — Google likes asking for this follow-up.
5. Always state: *what happens with duplicates, negatives, empty input, and integer overflow?*

---

## 1. Hashing & Counting

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1 | [Two Sum](https://leetcode.com/problems/two-sum/) | Easy | complement lookup | `G` `U` 🔥 |
| ☐ | 49 | [Group Anagrams](https://leetcode.com/problems/group-anagrams/) ⭐ | Medium | canonical key → group | `G` `U` 🔥 |
| ☐ | 128 | [Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) ⭐ | Medium | set; only start at sequence heads | `G` `U` 🔥 |
| ☐ | 347 | [Top K Frequent Elements](https://leetcode.com/problems/top-k-frequent-elements/) ⭐ | Medium | count + bucket sort (or heap) | `G` `U` 🔥 |
| ☐ | 36 | [Valid Sudoku](https://leetcode.com/problems/valid-sudoku/) | Medium | row/col/box sets | `G` `U` |
| ☐ | 229 | [Majority Element II](https://leetcode.com/problems/majority-element-ii/) | Medium | Boyer–Moore with 2 candidates | `G` |
| ☐ | 939 | [Minimum Area Rectangle](https://leetcode.com/problems/minimum-area-rectangle/) | Medium | hash points, pick diagonals | `G` |

## 2. Prefix Sums

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 238 | [Product of Array Except Self](https://leetcode.com/problems/product-of-array-except-self/) ⭐ | Medium | prefix × suffix products, O(1) extra | `G` `U` 🔥 |
| ☐ | 560 | [Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) ⭐ | Medium | prefix sum + hash map count | `G` `U` 🔥 |
| ☐ | 525 | [Contiguous Array](https://leetcode.com/problems/contiguous-array/) | Medium | map 0 → −1, longest zero-sum | `G` 🔥 |
| ☐ | 974 | [Subarray Sums Divisible by K](https://leetcode.com/problems/subarray-sums-divisible-by-k/) | Medium | count by prefix mod k | `G` `U` |
| ☐ | 523 | [Continuous Subarray Sum](https://leetcode.com/problems/continuous-subarray-sum/) | Medium | first index of each prefix mod k | `G` |
| ☐ | 304 | [Range Sum Query 2D - Immutable](https://leetcode.com/problems/range-sum-query-2d-immutable/) | Medium | 2D prefix sums | `G` |
| ☐ | 528 | [Random Pick with Weight](https://leetcode.com/problems/random-pick-with-weight/) ⭐ | Medium | prefix sums + binary search | `G` `U` |
| ☐ | 1915 | [Number of Wonderful Substrings](https://leetcode.com/problems/number-of-wonderful-substrings/) | Medium | prefix XOR bitmask + hash map | `G` `U` 🔥 |

## 3. Kadane & In-place Tricks

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 53 | [Maximum Subarray](https://leetcode.com/problems/maximum-subarray/) ⭐ | Medium | Kadane: extend or restart | `G` `U` 🔥 |
| ☐ | 189 | [Rotate Array](https://leetcode.com/problems/rotate-array/) | Medium | triple reverse | `G` 🔥 |
| ☐ | 31 | [Next Permutation](https://leetcode.com/problems/next-permutation/) ⭐ | Medium | find pivot, swap, reverse suffix | `G` `U` 🔥 |
| ☐ | 41 | [First Missing Positive](https://leetcode.com/problems/first-missing-positive/) ⭐ | Hard | place each value at index v−1 | `G` |
| ☐ | 169 | [Majority Element](https://leetcode.com/problems/majority-element/) | Easy | Boyer–Moore voting | `G` 🔥 |

## 4. Matrix Manipulation

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 54 | [Spiral Matrix](https://leetcode.com/problems/spiral-matrix/) ⭐ | Medium | shrink four boundaries | `G` `U` 🔥 |
| ☐ | 48 | [Rotate Image](https://leetcode.com/problems/rotate-image/) ⭐ | Medium | transpose + reverse rows | `G` `U` 🔥 |
| ☐ | 73 | [Set Matrix Zeroes](https://leetcode.com/problems/set-matrix-zeroes/) | Medium | first row/col as markers | `G` |
| ☐ | 289 | [Game of Life](https://leetcode.com/problems/game-of-life/) | Medium | encode next state in spare bits | `G` |
| ☐ | 1861 | [Rotating the Box](https://leetcode.com/problems/rotating-the-box/) | Medium | gravity per row, then rotate | `G` `U` |

---

**Interview-ready when:** you can derive the prefix-sum + hash-map recurrence for a new counting question without notes and always justify the hash map's O(1) average assumption.
