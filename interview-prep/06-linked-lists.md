# Linked Lists: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Linked-list algorithms](../docs/linked-lists/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 9 ⭐ problems first.

**Recognise it when:** the input is a `ListNode*`; the challenge is almost always pointer bookkeeping, not the algorithm.

1. **Dummy head** removes every "what if the head changes?" special case.
2. **Reverse in place** — memorise `prev, cur, next` and draw three boxes before coding.
3. **Fast/slow pointers** find the middle, detect cycles, and find the cycle entry (`142`).
4. **Split → process → merge** solves Reorder List, Palindrome List, and Sort List.
5. Say the **space** answer: recursion uses O(n) stack; interviewers usually want O(1) iterative.

---

## 1. Fundamentals

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 206 | [Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/) ⭐ | Easy | iterative + recursive | `G` `U` 🔥 |
| ☐ | 21 | [Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists/) ⭐ | Easy | dummy head merge | `G` 🔥 |
| ☐ | 876 | [Middle of the Linked List](https://leetcode.com/problems/middle-of-the-linked-list/) | Easy | fast/slow middle | `G` 🔥 |
| ☐ | 141 | [Linked List Cycle](https://leetcode.com/problems/linked-list-cycle/) | Easy | Floyd | `G` 🔥 |
| ☐ | 142 | [Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/) ⭐ | Medium | Floyd: reset one pointer to head | `G` |
| ☐ | 160 | [Intersection of Two Linked Lists](https://leetcode.com/problems/intersection-of-two-linked-lists/) | Easy | switch heads after reaching the end | `G` |
| ☐ | 19 | [Remove Nth Node From End of List](https://leetcode.com/problems/remove-nth-node-from-end-of-list/) | Medium | gap of n between pointers | `G` 🔥 |

## 2. Arithmetic & Rewiring

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 2 | [Add Two Numbers](https://leetcode.com/problems/add-two-numbers/) ⭐ | Medium | carry digit by digit | `G` `U` 🔥 |
| ☐ | 445 | [Add Two Numbers II](https://leetcode.com/problems/add-two-numbers-ii/) | Medium | stacks or reverse first | `G` |
| ☐ | 24 | [Swap Nodes in Pairs](https://leetcode.com/problems/swap-nodes-in-pairs/) | Medium | reverse in groups of 2 | `G` `U` |
| ☐ | 92 | [Reverse Linked List II](https://leetcode.com/problems/reverse-linked-list-ii/) | Medium | reverse a sub-range in place | `G` |
| ☐ | 25 | [Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group/) ⭐ | Hard | reverse k at a time, keep tail | `G` `U` 🔥 |
| ☐ | 61 | [Rotate List](https://leetcode.com/problems/rotate-list/) | Medium | close into a ring, cut at n − k | `G` |
| ☐ | 328 | [Odd Even Linked List](https://leetcode.com/problems/odd-even-linked-list/) | Medium | two chains, then join | `G` |
| ☐ | 86 | [Partition List](https://leetcode.com/problems/partition-list/) | Medium | two dummy lists | `G` |
| ☐ | 82 | [Remove Duplicates from Sorted List II](https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/) | Medium | skip whole duplicate runs | `G` |
| ☐ | 1171 | [Remove Zero Sum Consecutive Nodes from Linked List](https://leetcode.com/problems/remove-zero-sum-consecutive-nodes-from-linked-list/) | Medium | prefix sum → last node map | `G` `U` |

## 3. Split, Process, Merge

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 234 | [Palindrome Linked List](https://leetcode.com/problems/palindrome-linked-list/) | Easy | reverse second half | `G` 🔥 |
| ☐ | 143 | [Reorder List](https://leetcode.com/problems/reorder-list/) ⭐ | Medium | middle + reverse + interleave | `G` 🔥 |
| ☐ | 148 | [Sort List](https://leetcode.com/problems/sort-list/) ⭐ | Medium | merge sort, O(1) extra bottom-up | `G` 🔥 |
| ☐ | 23 | [Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists/) ⭐ | Hard | min-heap or pairwise merge | `G` `U` 🔥 |

## 4. Pointers to Other Structures

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 138 | [Copy List with Random Pointer](https://leetcode.com/problems/copy-list-with-random-pointer/) ⭐ | Medium | interleave copies or hash map | `G` `U` |
| ☐ | 430 | [Flatten a Multilevel Doubly Linked List](https://leetcode.com/problems/flatten-a-multilevel-doubly-linked-list/) | Medium | DFS with explicit stack | `G` |

---

**Interview-ready when:** you can reverse, split, and merge lists without drawing on paper and immediately name the O(1)-space version.
