# Data Structure Design (LLD-style Coding): Google / Uber Level

[← All interview topics](README.md) · Handbook: [Heaps and data-structure design](../docs/heaps-and-design/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 11 ⭐ problems first.

**Recognise it when:** "Design a class that supports …". Design classes are among **Uber's most-tagged questions** (First Unique Number, Hit Counter, LRU, In-Memory File System); Google asks them too (Snapshot Array, Logger Rate Limiter).

1. **Write the operations and their target complexities first**, then choose structures. Most answers combine a hash map with one ordered structure (list, heap, deque, `std::map`).
2. **Hash map + doubly linked list** = O(1) LRU / LFU / All O(1). Draw the pointers.
3. **Hash map + array (swap with last)** = O(1) random access + deletion.
4. **Time-based data:** store `(timestamp, value)` lists and binary search; for sliding time windows use a queue or circular buckets.
5. Discuss **concurrency, memory limits, and scaling** briefly at the end — this is where senior candidates score points.

---

## 1. Caches

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 146 | [LRU Cache](https://leetcode.com/problems/lru-cache/) ⭐ | Medium | hash map + doubly linked list | `G` `U` 🔥 |
| ☐ | 460 | [LFU Cache](https://leetcode.com/problems/lfu-cache/) ⭐ | Hard | key map + frequency → list map + minFreq | `G` `U` |
| ☐ | 432 | [All O\`one Data Structure](https://leetcode.com/problems/all-oone-data-structure/) | Hard | linked list of count buckets | `G` `U` |

## 2. Hash Map + Array / Queue

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 380 | [Insert Delete GetRandom O(1)](https://leetcode.com/problems/insert-delete-getrandom-o1/) ⭐ | Medium | array + index map, swap with last | `G` `U` |
| ☐ | 381 | [Insert Delete GetRandom O(1) - Duplicates allowed](https://leetcode.com/problems/insert-delete-getrandom-o1-duplicates-allowed/) | Hard | index sets per value | `G` `U` |
| ☐ | 1429 | [First Unique Number](https://leetcode.com/problems/first-unique-number/) ⭐ 🔒 | Medium | queue + counts (or ordered dict) | `G` `U` 🔥 |
| ☐ | 933 | [Number of Recent Calls](https://leetcode.com/problems/number-of-recent-calls/) | Easy | queue, drop old timestamps | `G` 🔥 |
| ☐ | 362 | [Design Hit Counter](https://leetcode.com/problems/design-hit-counter/) ⭐ 🔒 | Medium | circular buckets of size 300 | `G` `U` |
| ☐ | 359 | [Logger Rate Limiter](https://leetcode.com/problems/logger-rate-limiter/) 🔒 | Easy | map message → next allowed time | `G` 🔥 |

## 3. Versioned / Time-based Storage

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 981 | [Time Based Key-Value Store](https://leetcode.com/problems/time-based-key-value-store/) ⭐ | Medium | per-key timestamp list + binary search | `G` `U` 🔥 |
| ☐ | 1146 | [Snapshot Array](https://leetcode.com/problems/snapshot-array/) ⭐ | Medium | per-index list of (snap, value) | `G` |
| ☐ | 2034 | [Stock Price Fluctuation](https://leetcode.com/problems/stock-price-fluctuation/) | Medium | map + two heaps with lazy deletion | `G` |

## 4. Ordered Structures

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 2349 | [Design a Number Container System](https://leetcode.com/problems/design-a-number-container-system/) | Medium | map index → number, number → ordered set | `G` |
| ☐ | 855 | [Exam Room](https://leetcode.com/problems/exam-room/) | Medium | ordered set of seats, pick biggest gap | `G` `U` |
| ☐ | 1244 | [Design A Leaderboard](https://leetcode.com/problems/design-a-leaderboard/) 🔒 | Medium | map + sorting / multiset | `G` `U` |
| ☐ | 2353 | [Design a Food Rating System](https://leetcode.com/problems/design-a-food-rating-system/) | Medium | per-cuisine ordered set | `G` |

## 5. Stacks, Iterators & Editors

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 895 | [Maximum Frequency Stack](https://leetcode.com/problems/maximum-frequency-stack/) ⭐ | Hard | frequency → stack map | `G` `U` |
| ☐ | 1472 | [Design Browser History](https://leetcode.com/problems/design-browser-history/) | Medium | array + cursor | `G` `U` |
| ☐ | 2296 | [Design a Text Editor](https://leetcode.com/problems/design-a-text-editor/) | Hard | two stacks around the cursor | `G` `U` |
| ☐ | 341 | [Flatten Nested List Iterator](https://leetcode.com/problems/flatten-nested-list-iterator/) ⭐ | Medium | stack of iterators | `G` |
| ☐ | 284 | [Peeking Iterator](https://leetcode.com/problems/peeking-iterator/) | Medium | cache the next element | `G` |
| ☐ | 251 | [Flatten 2D Vector](https://leetcode.com/problems/flatten-2d-vector/) 🔒 | Medium | two indices | `G` |

## 6. Systems & Simulations

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 355 | [Design Twitter](https://leetcode.com/problems/design-twitter/) ⭐ | Medium | per-user tweets + k-way merge | `G` |
| ☐ | 1396 | [Design Underground System](https://leetcode.com/problems/design-underground-system/) ⭐ | Medium | two maps: check-ins + route totals | `G` |
| ☐ | 535 | [Encode and Decode TinyURL](https://leetcode.com/problems/encode-and-decode-tinyurl/) | Medium | counter / random id ↔ map | `G` `U` |
| ☐ | 2502 | [Design Memory Allocator](https://leetcode.com/problems/design-memory-allocator/) | Medium | array scan for free blocks | `G` `U` |
| ☐ | 353 | [Design Snake Game](https://leetcode.com/problems/design-snake-game/) 🔒 | Medium | deque body + set of cells | `G` `U` |
| ☐ | 348 | [Design Tic-Tac-Toe](https://leetcode.com/problems/design-tic-tac-toe/) 🔒 | Medium | row/col/diag counters | `G` |

---

**Interview-ready when:** you can state the target complexity of every operation, pick the structures in under 3 minutes, and code the class cleanly with no global state.
