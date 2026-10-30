# Greedy: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Monotonic structures and greedy §7–§8](../docs/monotonic-and-greedy/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 7 ⭐ problems first.

**Recognise it when:** a locally best choice never needs to be undone. If you can't prove that, it's probably DP.

1. **Prove it** with an *exchange argument* ("any optimal solution can be changed to include my choice without getting worse") or *stays ahead*. Interviewers at Google will ask.
2. **Sort first**: by end time, by ratio, by a custom comparator (Largest Number: `a+b > b+a`).
3. **Reach / layers** for jump games: track the farthest index reachable within the current number of jumps.
4. **Two passes** (left → right, then right → left) for neighbour constraints (Candy).
5. When greedy fails on a counter-example, fall back to DP — mention this trade-off explicitly.

---

## 1. Reachability & Jumps

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 55 | [Jump Game](https://leetcode.com/problems/jump-game/) ⭐ | Medium | track farthest reachable | `G` 🔥 |
| ☐ | 45 | [Jump Game II](https://leetcode.com/problems/jump-game-ii/) ⭐ | Medium | BFS layers by farthest | `G` `U` 🔥 |
| ☐ | 1326 | [Minimum Number of Taps to Open to Water a Garden](https://leetcode.com/problems/minimum-number-of-taps-to-open-to-water-a-garden/) | Hard | convert to jump game / interval cover | `G` `U` |
| ☐ | 1024 | [Video Stitching](https://leetcode.com/problems/video-stitching/) | Medium | interval cover, extend greedily | `G` |

## 2. Prefix Balance

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 134 | [Gas Station](https://leetcode.com/problems/gas-station/) ⭐ | Medium | reset start when tank < 0 | `G` |
| ☐ | 678 | [Valid Parenthesis String](https://leetcode.com/problems/valid-parenthesis-string/) ⭐ | Medium | track range [lo, hi] of open count | `G` 🔥 |

## 3. Two-pass Neighbour Constraints

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 135 | [Candy](https://leetcode.com/problems/candy/) ⭐ | Hard | left pass + right pass | `G` `U` |

## 4. Sorting & Custom Order

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 179 | [Largest Number](https://leetcode.com/problems/largest-number/) ⭐ | Medium | compare a+b vs b+a | `G` 🔥 |
| ☐ | 455 | [Assign Cookies](https://leetcode.com/problems/assign-cookies/) | Easy | sort both, match smallest | `G` |
| ☐ | 1029 | [Two City Scheduling](https://leetcode.com/problems/two-city-scheduling/) | Medium | sort by cost difference | `G` |
| ☐ | 763 | [Partition Labels](https://leetcode.com/problems/partition-labels/) | Medium | last occurrence, extend partition | `G` |
| ☐ | 1968 | [Array With Elements Not Equal to Average of Neighbors](https://leetcode.com/problems/array-with-elements-not-equal-to-average-of-neighbors/) | Medium | sort + interleave halves | `U` |

## 5. Counting & Grouping

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 846 | [Hand of Straights](https://leetcode.com/problems/hand-of-straights/) ⭐ | Medium | ordered map, start from smallest | `G` |
| ☐ | 659 | [Split Array into Consecutive Subsequences](https://leetcode.com/problems/split-array-into-consecutive-subsequences/) | Medium | count + tails map | `G` |
| ☐ | 1400 | [Construct K Palindrome Strings](https://leetcode.com/problems/construct-k-palindrome-strings/) | Medium | count odd frequencies | `G` `U` |
| ☐ | 605 | [Can Place Flowers](https://leetcode.com/problems/can-place-flowers/) | Easy | place greedily left to right | `G` |

## 6. Digits & Bits

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 670 | [Maximum Swap](https://leetcode.com/problems/maximum-swap/) | Medium | last index of each digit | `G` |
| ☐ | 1717 | [Maximum Score From Removing Substrings](https://leetcode.com/problems/maximum-score-from-removing-substrings/) | Medium | remove the higher-value pair first | `G` |
| ☐ | 330 | [Patching Array](https://leetcode.com/problems/patching-array/) | Hard | maintain covered range [1, miss) | `G` |

## 7. Hard Greedy

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 765 | [Couples Holding Hands](https://leetcode.com/problems/couples-holding-hands/) | Hard | swap partner in place / union-find | `G` |

---

**Interview-ready when:** you can give an exchange-argument proof in two sentences and produce a counter-example when a tempting greedy is wrong.
