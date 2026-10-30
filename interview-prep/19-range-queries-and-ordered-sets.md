# Range Queries: Fenwick Tree, Segment Tree & Ordered Sets: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Range queries and offline processing](../docs/range-queries/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 7 ⭐ problems first.

**Recognise it when:** point updates + range queries, "count of smaller elements before/after", inversions, or online insertion with "nearest neighbour" queries. Less frequent than graphs or DP, but it shows up in Google onsite rounds and Uber's recent tag (Block Placement Queries).

1. **Fenwick tree (BIT):** prefix sums with point updates in O(log n), about 10 lines. Use it for counting with coordinate compression.
2. **Segment tree:** any associative operation (max, min, sum, gcd); add lazy propagation for range updates.
3. **Merge sort counting** is an alternative for inversion-style questions (Reverse Pairs, Count of Smaller Numbers After Self, Count of Range Sum).
4. **Ordered set / map** (`std::set`, `lower_bound`) for predecessor/successor queries — often the simplest correct answer.
5. Interviewers usually accept a simpler O(n log n) idea; mention the tree only if it is genuinely needed.

---

## 1. Fenwick / Segment Tree Basics

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 307 | [Range Sum Query - Mutable](https://leetcode.com/problems/range-sum-query-mutable/) ⭐ | Medium | implement BIT and segment tree | `G` |
| ☐ | 308 | [Range Sum Query 2D - Mutable](https://leetcode.com/problems/range-sum-query-2d-mutable/) 🔒 | Medium | 2D Fenwick tree | `G` |

## 2. Counting with Order Statistics

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 315 | [Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/) ⭐ | Hard | BIT over ranks, scan right to left | `G` |
| ☐ | 493 | [Reverse Pairs](https://leetcode.com/problems/reverse-pairs/) ⭐ | Hard | merge sort count or BIT | `G` |
| ☐ | 327 | [Count of Range Sum](https://leetcode.com/problems/count-of-range-sum/) | Hard | merge sort on prefix sums | `G` |
| ☐ | 2426 | [Number of Pairs Satisfying Inequality](https://leetcode.com/problems/number-of-pairs-satisfying-inequality/) | Hard | BIT on a[i] − b[i] differences | `G` |
| ☐ | 2179 | [Count Good Triplets in an Array](https://leetcode.com/problems/count-good-triplets-in-an-array/) | Hard | map to positions + BIT both sides | `G` |
| ☐ | 406 | [Queue Reconstruction by Height](https://leetcode.com/problems/queue-reconstruction-by-height/) | Medium | sort, insert by k (or BIT k-th empty) | `G` |

## 3. Segment Tree on Values / Coordinates

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 2407 | [Longest Increasing Subsequence II](https://leetcode.com/problems/longest-increasing-subsequence-ii/) ⭐ | Hard | max segment tree over values | `G` |
| ☐ | 699 | [Falling Squares](https://leetcode.com/problems/falling-squares/) | Hard | coordinate compression + range max | `U` |
| ☐ | 3161 | [Block Placement Queries](https://leetcode.com/problems/block-placement-queries/) ⭐ | Hard | segment tree of max gap | `G` `U` |
| ☐ | 2286 | [Booking Concert Tickets in Groups](https://leetcode.com/problems/booking-concert-tickets-in-groups/) | Hard | max + sum segment tree | `G` |
| ☐ | 683 | [K Empty Slots](https://leetcode.com/problems/k-empty-slots/) 🔒 | Hard | position array + sliding window | `G` |

## 4. Ordered Set / Map

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 220 | [Contains Duplicate III](https://leetcode.com/problems/contains-duplicate-iii/) ⭐ | Hard | ordered set or buckets of width t + 1 | `G` |
| ☐ | 975 | [Odd Even Jump](https://leetcode.com/problems/odd-even-jump/) ⭐ | Hard | ordered map for next jumps + DP | `G` |
| ☐ | 1818 | [Minimum Absolute Sum Difference](https://leetcode.com/problems/minimum-absolute-sum-difference/) | Medium | sorted copy + binary search | `G` `U` |

---

**Interview-ready when:** you can write a Fenwick tree from memory and decide in under a minute whether an ordered set, BIT, or merge sort is the simplest correct tool.
