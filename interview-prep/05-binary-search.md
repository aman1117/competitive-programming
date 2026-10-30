# Binary Search: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Search, prefixes, and windows](../docs/search-and-windows/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 9 ⭐ problems first.

**Recognise it when:** the input is sorted/rotated, you need the first index where a predicate becomes true, or the answer is a number in a range and **"can we do it with X?" is monotone** (binary search on the answer).

```text
lo, hi = smallest possible, largest possible     # answer is in [lo, hi]
while lo < hi:
    mid = lo + (hi - lo) / 2
    if feasible(mid): hi = mid                    # mid might be the answer
    else:             lo = mid + 1
return lo
```

1. Define a **predicate** `false…false true…true` and search for the first `true`. Every variant reduces to this.
2. **Search on answer:** write `feasible(x)` (usually greedy, O(n)), then argue monotonicity out loud.
3. **Rotated arrays:** one half is always sorted — decide which, then check whether the target lies inside it.
4. **Grid + binary search** (Safest Path, Spreading Fire, Last Day to Cross): binary search the time/threshold, BFS/DSU as the check. These are recent Uber/Google favourites.
5. See also: Split Array Largest Sum (410) and Maximum Profit in Job Scheduling (1235) in the [DP list](15-dynamic-programming.md).

---

## 1. Classic Index Search

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 704 | [Binary Search](https://leetcode.com/problems/binary-search/) | Easy | warm-up | `G` 🔥 |
| ☐ | 35 | [Search Insert Position](https://leetcode.com/problems/search-insert-position/) | Easy | lower_bound | `G` 🔥 |
| ☐ | 34 | [Find First and Last Position of Element in Sorted Array](https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/) ⭐ | Medium | lower_bound and upper_bound | `G` `U` 🔥 |
| ☐ | 69 | [Sqrt(x)](https://leetcode.com/problems/sqrtx/) | Easy | largest x with x² ≤ n | `G` `U` 🔥 |
| ☐ | 540 | [Single Element in a Sorted Array](https://leetcode.com/problems/single-element-in-a-sorted-array/) | Medium | pair parity decides side | `G` |
| ☐ | 1428 | [Leftmost Column with at Least a One](https://leetcode.com/problems/leftmost-column-with-at-least-a-one/) 🔒 | Medium | move left/down from top-right, or BS per row | `U` 🔥 |

## 2. Rotated & Unimodal Arrays

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 33 | [Search in Rotated Sorted Array](https://leetcode.com/problems/search-in-rotated-sorted-array/) ⭐ | Medium | find the sorted half | `G` `U` 🔥 |
| ☐ | 153 | [Find Minimum in Rotated Sorted Array](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/) ⭐ | Medium | compare mid with right end | `G` `U` 🔥 |
| ☐ | 162 | [Find Peak Element](https://leetcode.com/problems/find-peak-element/) ⭐ | Medium | go towards the larger neighbour | `G` `U` 🔥 |

## 3. 2D & Two Arrays

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 74 | [Search a 2D Matrix](https://leetcode.com/problems/search-a-2d-matrix/) | Medium | treat as 1D | `G` `U` |
| ☐ | 240 | [Search a 2D Matrix II](https://leetcode.com/problems/search-a-2d-matrix-ii/) | Medium | staircase from top-right | `G` |
| ☐ | 4 | [Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays/) ⭐ | Hard | partition the smaller array | `G` `U` 🔥 |
| ☐ | 658 | [Find K Closest Elements](https://leetcode.com/problems/find-k-closest-elements/) | Medium | binary search left edge of window | `G` `U` |

## 4. Binary Search on the Answer

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 875 | [Koko Eating Bananas](https://leetcode.com/problems/koko-eating-bananas/) ⭐ | Medium | feasible(speed) | `G` `U` 🔥 |
| ☐ | 1011 | [Capacity To Ship Packages Within D Days](https://leetcode.com/problems/capacity-to-ship-packages-within-d-days/) ⭐ | Medium | feasible(capacity) | `G` `U` |
| ☐ | 1482 | [Minimum Number of Days to Make m Bouquets](https://leetcode.com/problems/minimum-number-of-days-to-make-m-bouquets/) | Medium | feasible(day) | `G` |
| ☐ | 1283 | [Find the Smallest Divisor Given a Threshold](https://leetcode.com/problems/find-the-smallest-divisor-given-a-threshold/) | Medium | feasible(divisor) | `G` |
| ☐ | 774 | [Minimize Max Distance to Gas Station](https://leetcode.com/problems/minimize-max-distance-to-gas-station/) 🔒 | Hard | real-valued search on distance | `G` |
| ☐ | 2333 | [Minimum Sum of Squared Difference](https://leetcode.com/problems/minimum-sum-of-squared-difference/) | Medium | search the final ceiling value | `G` `U` 🔥 |

## 5. Kth Smallest by Counting

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 378 | [Kth Smallest Element in a Sorted Matrix](https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix/) | Medium | count ≤ mid per row | `G` |
| ☐ | 668 | [Kth Smallest Number in Multiplication Table](https://leetcode.com/problems/kth-smallest-number-in-multiplication-table/) | Hard | count ≤ mid in table | `G` |
| ☐ | 719 | [Find K-th Smallest Pair Distance](https://leetcode.com/problems/find-k-th-smallest-pair-distance/) ⭐ | Hard | sort + window count ≤ mid | `G` `U` |

## 6. Binary Search + Graph Check

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 2258 | [Escape the Spreading Fire](https://leetcode.com/problems/escape-the-spreading-fire/) ⭐ | Hard | search wait time, BFS fire vs person | `G` `U` 🔥 |
| ☐ | 1970 | [Last Day Where You Can Still Cross](https://leetcode.com/problems/last-day-where-you-can-still-cross/) | Hard | search day, BFS/DSU connectivity | `G` |

---

**Interview-ready when:** you can write the template without off-by-one errors, name the predicate, and prove its monotonicity before coding.
