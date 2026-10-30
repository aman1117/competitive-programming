# Heaps & Priority Queues: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Heaps and data-structure design](../docs/heaps-and-design/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 10 ⭐ problems first.

**Recognise it when:** "top / kth / k closest", "merge k sorted things", a running median, or a simulation where you repeatedly take the best available option.

1. **Top-k:** keep a heap of size `k` with the *opposite* order (min-heap for k largest) → O(n log k). Mention quickselect for average O(n).
2. **Two heaps:** max-heap for the lower half, min-heap for the upper half; rebalance so sizes differ by ≤ 1.
3. **K-way merge:** push the head of each list as `(value, list, index)`.
4. **Scheduling simulation:** sort events by time, push "available" items into a heap, pop the best — CPU, servers, meeting rooms.
5. **Greedy with regret:** take greedily, keep choices in a heap, undo the worst when you run out (Refueling Stops, Furthest Building, Course Schedule III).
6. C++ `priority_queue` is a max-heap; use `greater<>` for a min-heap. No decrease-key → push duplicates and skip stale entries (lazy deletion).

---

## 1. Top-K

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 215 | [Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/) ⭐ | Medium | size-k min-heap or quickselect | `G` `U` 🔥 |
| ☐ | 703 | [Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream/) | Easy | size-k min-heap | `G` |
| ☐ | 973 | [K Closest Points to Origin](https://leetcode.com/problems/k-closest-points-to-origin/) ⭐ | Medium | size-k max-heap on distance | `G` |
| ☐ | 692 | [Top K Frequent Words](https://leetcode.com/problems/top-k-frequent-words/) ⭐ | Medium | heap with custom tie-break | `G` `U` |
| ☐ | 767 | [Reorganize String](https://leetcode.com/problems/reorganize-string/) ⭐ | Medium | always place the most frequent | `G` |

## 2. Two Heaps

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 295 | [Find Median from Data Stream](https://leetcode.com/problems/find-median-from-data-stream/) ⭐ | Hard | lower max-heap + upper min-heap | `G` `U` |
| ☐ | 480 | [Sliding Window Median](https://leetcode.com/problems/sliding-window-median/) | Hard | two heaps + lazy deletion | `G` |
| ☐ | 502 | [IPO](https://leetcode.com/problems/ipo/) | Hard | unlocked projects into a max-heap | `G` `U` |

## 3. K-way Merge

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 373 | [Find K Pairs with Smallest Sums](https://leetcode.com/problems/find-k-pairs-with-smallest-sums/) ⭐ | Medium | push (i, 0), advance j | `G` `U` |
| ☐ | 632 | [Smallest Range Covering Elements from K Lists](https://leetcode.com/problems/smallest-range-covering-elements-from-k-lists/) ⭐ | Hard | heap + current max | `G` |

## 4. Scheduling Simulations

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 621 | [Task Scheduler](https://leetcode.com/problems/task-scheduler/) ⭐ | Medium | most frequent first / formula | `G` `U` 🔥 |
| ☐ | 1834 | [Single-Threaded CPU](https://leetcode.com/problems/single-threaded-cpu/) ⭐ | Medium | sort by time, heap by (duration, index) | `G` |
| ☐ | 1882 | [Process Tasks Using Servers](https://leetcode.com/problems/process-tasks-using-servers/) | Medium | free heap + busy heap | `G` |
| ☐ | 358 | [Rearrange String k Distance Apart](https://leetcode.com/problems/rearrange-string-k-distance-apart/) 🔒 | Hard | heap + cooldown queue | `G` |

## 5. Greedy with Regret

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 871 | [Minimum Number of Refueling Stops](https://leetcode.com/problems/minimum-number-of-refueling-stops/) ⭐ | Hard | max-heap of passed stations | `G` `U` 🔥 |
| ☐ | 1642 | [Furthest Building You Can Reach](https://leetcode.com/problems/furthest-building-you-can-reach/) | Medium | min-heap of ladder climbs | `G` `U` |
| ☐ | 630 | [Course Schedule III](https://leetcode.com/problems/course-schedule-iii/) | Hard | sort by deadline, drop longest | `G` |
| ☐ | 857 | [Minimum Cost to Hire K Workers](https://leetcode.com/problems/minimum-cost-to-hire-k-workers/) | Hard | sort by ratio, max-heap of quality | `G` |
| ☐ | 1383 | [Maximum Performance of a Team](https://leetcode.com/problems/maximum-performance-of-a-team/) | Hard | sort by efficiency, min-heap of speed | `G` |

## 6. Heaps with Lazy Deletion

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1825 | [Finding MK Average](https://leetcode.com/problems/finding-mk-average/) | Hard | three multisets / heaps | `G` |

---

**Interview-ready when:** you can justify O(n log k) vs O(n log n), implement lazy deletion, and recognise when a scheduling question is really "sort + heap".
