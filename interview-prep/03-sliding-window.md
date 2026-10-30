# Sliding Window: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Search, prefixes, and windows](../docs/search-and-windows/README.md), [Monotonic structures §2 (window max)](../docs/monotonic-and-greedy/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 9 ⭐ problems first.

**Recognise it when:** you want the longest/shortest/count of **contiguous** subarrays or substrings satisfying a condition that is *monotone* (shrinking a valid window keeps it valid, or growing an invalid one keeps it invalid).

```text
for right in 0..n-1:
    add(a[right])
    while window is invalid:      # or: while valid, for "shortest"
        remove(a[left]); left++
    update answer
```

1. **Longest valid:** shrink while invalid, record after the loop. **Shortest valid:** record *inside* the shrink loop.
2. **Exactly K** = `atMost(K) − atMost(K−1)`.
3. Windows **don't work with negative numbers** for sum targets → use prefix sums + hash map (or a monotonic deque, see 862).
4. **Window max/min** → monotonic deque storing indices. Uber's top sliding-window question (1438) needs two deques.

---

## 1. Fixed-size Window

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 643 | [Maximum Average Subarray I](https://leetcode.com/problems/maximum-average-subarray-i/) | Easy | warm-up running sum | `G` `U` |
| ☐ | 567 | [Permutation in String](https://leetcode.com/problems/permutation-in-string/) ⭐ | Medium | count match of 26 letters | `G` |
| ☐ | 438 | [Find All Anagrams in a String](https://leetcode.com/problems/find-all-anagrams-in-a-string/) | Medium | same as 567, collect starts | `G` |
| ☐ | 1423 | [Maximum Points You Can Obtain from Cards](https://leetcode.com/problems/maximum-points-you-can-obtain-from-cards/) | Medium | take from ends = keep a middle window | `G` `U` |
| ☐ | 2461 | [Maximum Sum of Distinct Subarrays With Length K](https://leetcode.com/problems/maximum-sum-of-distinct-subarrays-with-length-k/) | Medium | fixed window + distinct count | `G` |

## 2. Variable Window: Longest Valid

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 3 | [Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters/) ⭐ | Medium | last-seen index jump | `G` `U` 🔥 |
| ☐ | 424 | [Longest Repeating Character Replacement](https://leetcode.com/problems/longest-repeating-character-replacement/) ⭐ | Medium | window − maxFreq ≤ k | `G` 🔥 |
| ☐ | 904 | [Fruit Into Baskets](https://leetcode.com/problems/fruit-into-baskets/) | Medium | at most 2 distinct | `G` 🔥 |
| ☐ | 1004 | [Max Consecutive Ones III](https://leetcode.com/problems/max-consecutive-ones-iii/) ⭐ | Medium | at most k zeros | `G` 🔥 |
| ☐ | 1838 | [Frequency of the Most Frequent Element](https://leetcode.com/problems/frequency-of-the-most-frequent-element/) | Medium | sort + window cost ≤ k | `G` `U` |
| ☐ | 1658 | [Minimum Operations to Reduce X to Zero](https://leetcode.com/problems/minimum-operations-to-reduce-x-to-zero/) | Medium | longest middle with sum total − x | `G` 🔥 |

## 3. Variable Window: Shortest Valid

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 209 | [Minimum Size Subarray Sum](https://leetcode.com/problems/minimum-size-subarray-sum/) | Medium | shrink while sum ≥ target | `G` 🔥 |
| ☐ | 76 | [Minimum Window Substring](https://leetcode.com/problems/minimum-window-substring/) ⭐ | Hard | need/have counters | `G` `U` |

## 4. Counting Subarrays

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 713 | [Subarray Product Less Than K](https://leetcode.com/problems/subarray-product-less-than-k/) | Medium | add right − left + 1 per step | `G` 🔥 |
| ☐ | 930 | [Binary Subarrays With Sum](https://leetcode.com/problems/binary-subarrays-with-sum/) | Medium | atMost(goal) − atMost(goal − 1) | `G` 🔥 |
| ☐ | 992 | [Subarrays with K Different Integers](https://leetcode.com/problems/subarrays-with-k-different-integers/) ⭐ | Hard | exactly K = atMost(K) − atMost(K−1) | `G` `U` |
| ☐ | 1358 | [Number of Substrings Containing All Three Characters](https://leetcode.com/problems/number-of-substrings-containing-all-three-characters/) | Medium | count windows containing all 3 | `G` |

## 5. Monotonic Deque Windows

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 239 | [Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/) ⭐ | Hard | decreasing deque of indices | `G` `U` 🔥 |
| ☐ | 1438 | [Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit](https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/) ⭐ | Medium | max-deque + min-deque | `G` `U` 🔥 |
| ☐ | 2444 | [Count Subarrays With Fixed Bounds](https://leetcode.com/problems/count-subarrays-with-fixed-bounds/) | Hard | track last bad / last min / last max | `G` `U` |
| ☐ | 862 | [Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/) ⭐ | Hard | prefix sums + increasing deque (negatives!) | `G` |
| ☐ | 2009 | [Minimum Number of Operations to Make Array Continuous](https://leetcode.com/problems/minimum-number-of-operations-to-make-array-continuous/) | Hard | sort unique + window of width n | `G` `U` |

## 6. Hard Windows on Strings

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 30 | [Substring with Concatenation of All Words](https://leetcode.com/problems/substring-with-concatenation-of-all-words/) | Hard | word-sized steps, k offsets | `G` |

---

**Interview-ready when:** you can state the monotonicity property that makes a window valid, and you immediately switch to prefix sums when negatives appear.
