# Two Pointers: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Search, prefixes, and windows](../docs/search-and-windows/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 7 ⭐ problems first.

**Recognise it when:** the input is sorted (or can be sorted), you are pairing from both ends, or you must compact/partition an array in place.

1. **Opposite ends:** move the pointer whose value *cannot* be part of a better answer (Container With Most Water: move the shorter wall).
2. **Read/write pointers:** `write` marks the end of the kept prefix; `read` scans everything.
3. **Fast/slow:** cycle detection on a functional graph (`i → nums[i]`) is Floyd, exactly like a linked list.
4. **k-Sum:** sort, fix `k−2` elements, two-pointer the rest; skip duplicates *after* using a value.
5. Prove the move: say out loud which candidate pairs you are discarding and why none of them can be optimal.

---

## 1. Opposite Ends

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 167 | [Two Sum II - Input Array Is Sorted](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/) | Medium | classic sorted pair | `G` 🔥 |
| ☐ | 15 | [3Sum](https://leetcode.com/problems/3sum/) ⭐ | Medium | sort + fix one + two pointers, skip dups | `G` 🔥 |
| ☐ | 16 | [3Sum Closest](https://leetcode.com/problems/3sum-closest/) | Medium | same as 3Sum, track closest | `G` 🔥 |
| ☐ | 18 | [4Sum](https://leetcode.com/problems/4sum/) | Medium | generalise to k-Sum | `G` 🔥 |
| ☐ | 11 | [Container With Most Water](https://leetcode.com/problems/container-with-most-water/) ⭐ | Medium | move the shorter wall | `G` `U` 🔥 |
| ☐ | 42 | [Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/) ⭐ | Hard | left/right max, move smaller side | `G` `U` 🔥 |
| ☐ | 977 | [Squares of a Sorted Array](https://leetcode.com/problems/squares-of-a-sorted-array/) ⭐ | Easy | fill result from the back | `G` `U` 🔥 |
| ☐ | 881 | [Boats to Save People](https://leetcode.com/problems/boats-to-save-people/) | Medium | heaviest + lightest pairing | `G` |

## 2. Same Direction (Read / Write)

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 26 | [Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array/) | Easy | write pointer for unique values | `G` 🔥 |
| ☐ | 283 | [Move Zeroes](https://leetcode.com/problems/move-zeroes/) | Easy | stable compaction | `G` `U` 🔥 |
| ☐ | 88 | [Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array/) | Easy | merge from the back | `G` 🔥 |
| ☐ | 443 | [String Compression](https://leetcode.com/problems/string-compression/) | Medium | run-length encode in place | `G` 🔥 |
| ☐ | 844 | [Backspace String Compare](https://leetcode.com/problems/backspace-string-compare/) | Easy | scan backwards, skip counts | `G` |

## 3. Partitioning

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 75 | [Sort Colors](https://leetcode.com/problems/sort-colors/) ⭐ | Medium | Dutch national flag (3 pointers) | `G` `U` |
| ☐ | 680 | [Valid Palindrome II](https://leetcode.com/problems/valid-palindrome-ii/) ⭐ | Easy | one mismatch → try both skips | `G` `U` 🔥 |
| ☐ | 125 | [Valid Palindrome](https://leetcode.com/problems/valid-palindrome/) | Easy | skip non-alphanumerics | `G` `U` 🔥 |

## 4. Fast / Slow Pointers on Arrays

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 287 | [Find the Duplicate Number](https://leetcode.com/problems/find-the-duplicate-number/) ⭐ | Medium | Floyd on index graph | `G` 🔥 |
| ☐ | 202 | [Happy Number](https://leetcode.com/problems/happy-number/) | Easy | Floyd on digit-square sequence | `G` `U` 🔥 |

---

**Interview-ready when:** you can explain, for every pointer move, which pairs are pruned and why the remaining search space still contains the answer.
