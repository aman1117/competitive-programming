# Interview Prep: Google / Uber Level DSA

Topic-by-topic LeetCode practice lists for Google and Uber software-engineering interviews. Each topic file contains a short "how to practice" guide, the patterns you must recognise, and a curated problem table with progress checkboxes.

The theory behind each pattern lives in the [Algorithm handbook](../docs/README.md); every topic file links to the relevant chapters.

---

## How to use these lists

1. **Work through the topics in order** (or follow the [12-week plan](#suggested-12-week-plan-about-2-hours-a-day)). Earlier topics are prerequisites for later ones.
2. **Do every ⭐ problem first.** If you are short on time, the 219 ⭐ problems cover every pattern listed here.
3. **Mark progress** by changing `☐` to `✅` in the Done column.
4. For each problem: solve it untimed, then re-solve each ⭐ problem after 7 days without looking at your old code.
5. **Before an interview**, do the [company hot lists](#hot-right-now) below under timed conditions.

**Legend** (used in every file)

| Mark | Meaning |
|---|---|
| ⭐ | Must do: the clearest example of a core pattern, or very frequently asked |
| 🔒 | LeetCode Premium. 40 of the 557 problems; each section has free problems that cover the same pattern |
| `G` / `U` | Appears in the Google / Uber LeetCode company tag |
| 🔥 | Hot right now: high frequency on Google's last-3-months tag (score ≥ 25), or on Uber's last-6-months tag |
| — | Not company-tagged; included because it teaches a pattern that the tagged problems require |

---

## Topics

| # | Topic | Problems | ⭐ | Why it matters for Google / Uber |
|---|---|---|---|---|
| 01 | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) | 25 | 11 | Most common warm-up and first-round material at both companies |
| 02 | [Two Pointers](02-two-pointers.md) | 18 | 7 | Google favourite (3Sum, Trapping Rain Water); Uber asks 977 a lot |
| 03 | [Sliding Window](03-sliding-window.md) | 23 | 9 | Google staple; Uber's 1438 is one of its most-asked problems |
| 04 | [Stacks & Monotonic Stack](04-stacks-and-monotonic-stack.md) | 28 | 12 | Calculators, parsing, and "next greater" variants |
| 05 | [Binary Search](05-binary-search.md) | 24 | 9 | Binary search on the answer + grid checks are recent favourites |
| 06 | [Linked Lists](06-linked-lists.md) | 23 | 9 | Short pointer questions; Add Two Numbers is Google's #2 problem |
| 07 | [Binary Trees & BSTs](07-trees-and-bst.md) | 37 | 13 | Every loop has one; Uber asks Quad Tree and Kth Smallest in BST |
| 08 | [Tries (Prefix Trees)](08-tries.md) | 19 | 7 | Word Search II, autocomplete, and file-system design (Uber) |
| 09 | [Heaps & Priority Queues](09-heaps-and-priority-queues.md) | 20 | 10 | Top-k, scheduling simulations, and greedy-with-regret |
| 10 | [Intervals & Sweep Line](10-intervals-and-sweep-line.md) | 22 | 10 | Meetings/rides/bookings; Meeting Rooms II/III at both |
| 11 | [Greedy](11-greedy.md) | 20 | 7 | Needs a proof; Google interviewers ask "why is greedy correct?" |
| 12 | [Backtracking & Recursion](12-backtracking.md) | 24 | 9 | Generate-all and board search; Word Search is hot at Uber |
| 13 | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) | 39 | 14 | **Uber's most-tagged topic**: Bus Routes, Islands, Alien Dictionary |
| 14 | [Advanced Graphs: Dijkstra, Union-Find, MST](14-advanced-graphs.md) | 33 | 14 | Dijkstra, DSU (Islands II), MST; frequent at Uber and in Google onsites |
| 15 | [Dynamic Programming](15-dynamic-programming.md) | 107 | 38 | Hardest common round at Google; Uber asks routing/scheduling DP |
| 16 | [Bit Manipulation, Math & Randomization](16-bit-manipulation-and-math.md) | 31 | 13 | Overflow traps, fast power, and randomized algorithms |
| 17 | [Strings: Parsing, Matching & Hashing](17-strings.md) | 20 | 9 | Simulation (Text Justification), KMP, and rolling hash |
| 18 | [Data Structure Design (LLD-style Coding)](18-data-structure-design.md) | 28 | 11 | **Among Uber's most-tagged problems**: LRU, Hit Counter, First Unique |
| 19 | [Range Queries: Fenwick Tree, Segment Tree & Ordered Sets](19-range-queries-and-ordered-sets.md) | 16 | 7 | Occasional Google onsite Hard; know at least Fenwick tree |
| | **Total** | **557** | **219** | |

---

## Hot right now

These are the selected problems with the highest **recent** company-tag frequency. Use them as your last-week revision list for a specific company.

### Uber (ranked by last-6-months, then all-time frequency)

| # | Problem | Level | Topic file |
|---|---|---|---|
| 305 | [Number of Islands II](https://leetcode.com/problems/number-of-islands-ii/) 🔒 | Hard | [Advanced Graphs: Dijkstra, Union-Find, MST](14-advanced-graphs.md) |
| 1429 | [First Unique Number](https://leetcode.com/problems/first-unique-number/) 🔒 | Medium | [Data Structure Design (LLD-style Coding)](18-data-structure-design.md) |
| 815 | [Bus Routes](https://leetcode.com/problems/bus-routes/) | Hard | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) |
| 977 | [Squares of a Sorted Array](https://leetcode.com/problems/squares-of-a-sorted-array/) | Easy | [Two Pointers](02-two-pointers.md) |
| 269 | [Alien Dictionary](https://leetcode.com/problems/alien-dictionary/) 🔒 | Hard | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) |
| 427 | [Construct Quad Tree](https://leetcode.com/problems/construct-quad-tree/) | Medium | [Binary Trees & BSTs](07-trees-and-bst.md) |
| 2858 | [Minimum Edge Reversals So Every Node Is Reachable](https://leetcode.com/problems/minimum-edge-reversals-so-every-node-is-reachable/) | Hard | [Dynamic Programming](15-dynamic-programming.md) |
| 1438 | [Longest Continuous Subarray With Absolute Diff Less Than or Equal to Limit](https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/) | Medium | [Sliding Window](03-sliding-window.md) |
| 200 | [Number of Islands](https://leetcode.com/problems/number-of-islands/) | Medium | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) |
| 230 | [Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst/) | Medium | [Binary Trees & BSTs](07-trees-and-bst.md) |
| 1101 | [The Earliest Moment When Everyone Become Friends](https://leetcode.com/problems/the-earliest-moment-when-everyone-become-friends/) 🔒 | Medium | [Advanced Graphs: Dijkstra, Union-Find, MST](14-advanced-graphs.md) |
| 1475 | [Final Prices With a Special Discount in a Shop](https://leetcode.com/problems/final-prices-with-a-special-discount-in-a-shop/) | Easy | [Stacks & Monotonic Stack](04-stacks-and-monotonic-stack.md) |
| 79 | [Word Search](https://leetcode.com/problems/word-search/) | Medium | [Backtracking & Recursion](12-backtracking.md) |
| 564 | [Find the Closest Palindrome](https://leetcode.com/problems/find-the-closest-palindrome/) | Hard | [Bit Manipulation, Math & Randomization](16-bit-manipulation-and-math.md) |
| 827 | [Making A Large Island](https://leetcode.com/problems/making-a-large-island/) | Hard | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) |
| 1428 | [Leftmost Column with at Least a One](https://leetcode.com/problems/leftmost-column-with-at-least-a-one/) 🔒 | Medium | [Binary Search](05-binary-search.md) |
| 212 | [Word Search II](https://leetcode.com/problems/word-search-ii/) | Hard | [Tries (Prefix Trees)](08-tries.md) |
| 871 | [Minimum Number of Refueling Stops](https://leetcode.com/problems/minimum-number-of-refueling-stops/) | Hard | [Heaps & Priority Queues](09-heaps-and-priority-queues.md) |
| 588 | [Design In-Memory File System](https://leetcode.com/problems/design-in-memory-file-system/) 🔒 | Hard | [Tries (Prefix Trees)](08-tries.md) |
| 17 | [Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number/) | Medium | [Backtracking & Recursion](12-backtracking.md) |
| 2258 | [Escape the Spreading Fire](https://leetcode.com/problems/escape-the-spreading-fire/) | Hard | [Binary Search](05-binary-search.md) |
| 2812 | [Find the Safest Path in a Grid](https://leetcode.com/problems/find-the-safest-path-in-a-grid/) | Medium | [Advanced Graphs: Dijkstra, Union-Find, MST](14-advanced-graphs.md) |
| 384 | [Shuffle an Array](https://leetcode.com/problems/shuffle-an-array/) | Medium | [Bit Manipulation, Math & Randomization](16-bit-manipulation-and-math.md) |
| 2571 | [Minimum Operations to Reduce an Integer to 0](https://leetcode.com/problems/minimum-operations-to-reduce-an-integer-to-0/) | Medium | [Bit Manipulation, Math & Randomization](16-bit-manipulation-and-math.md) |
| 410 | [Split Array Largest Sum](https://leetcode.com/problems/split-array-largest-sum/) | Hard | [Dynamic Programming](15-dynamic-programming.md) |

### Google (Medium/Hard, ranked by last-3-months frequency)

| # | Problem | Level | Topic file |
|---|---|---|---|
| 2 | [Add Two Numbers](https://leetcode.com/problems/add-two-numbers/) | Medium | [Linked Lists](06-linked-lists.md) |
| 4 | [Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays/) | Hard | [Binary Search](05-binary-search.md) |
| 3 | [Longest Substring Without Repeating Characters](https://leetcode.com/problems/longest-substring-without-repeating-characters/) | Medium | [Sliding Window](03-sliding-window.md) |
| 15 | [3Sum](https://leetcode.com/problems/3sum/) | Medium | [Two Pointers](02-two-pointers.md) |
| 42 | [Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/) | Hard | [Two Pointers](02-two-pointers.md) |
| 128 | [Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 11 | [Container With Most Water](https://leetcode.com/problems/container-with-most-water/) | Medium | [Two Pointers](02-two-pointers.md) |
| 7 | [Reverse Integer](https://leetcode.com/problems/reverse-integer/) | Medium | [Bit Manipulation, Math & Randomization](16-bit-manipulation-and-math.md) |
| 5 | [Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/) | Medium | [Dynamic Programming](15-dynamic-programming.md) |
| 49 | [Group Anagrams](https://leetcode.com/problems/group-anagrams/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 53 | [Maximum Subarray](https://leetcode.com/problems/maximum-subarray/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 560 | [Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 875 | [Koko Eating Bananas](https://leetcode.com/problems/koko-eating-bananas/) | Medium | [Binary Search](05-binary-search.md) |
| 200 | [Number of Islands](https://leetcode.com/problems/number-of-islands/) | Medium | [Graphs: BFS, DFS & Topological Sort](13-graphs-bfs-dfs-topological-sort.md) |
| 410 | [Split Array Largest Sum](https://leetcode.com/problems/split-array-largest-sum/) | Hard | [Dynamic Programming](15-dynamic-programming.md) |
| 17 | [Letter Combinations of a Phone Number](https://leetcode.com/problems/letter-combinations-of-a-phone-number/) | Medium | [Backtracking & Recursion](12-backtracking.md) |
| 347 | [Top K Frequent Elements](https://leetcode.com/problems/top-k-frequent-elements/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 22 | [Generate Parentheses](https://leetcode.com/problems/generate-parentheses/) | Medium | [Backtracking & Recursion](12-backtracking.md) |
| 56 | [Merge Intervals](https://leetcode.com/problems/merge-intervals/) | Medium | [Intervals & Sweep Line](10-intervals-and-sweep-line.md) |
| 239 | [Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/) | Hard | [Sliding Window](03-sliding-window.md) |
| 6 | [Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion/) | Medium | [Strings: Parsing, Matching & Hashing](17-strings.md) |
| 198 | [House Robber](https://leetcode.com/problems/house-robber/) | Medium | [Dynamic Programming](15-dynamic-programming.md) |
| 54 | [Spiral Matrix](https://leetcode.com/problems/spiral-matrix/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 189 | [Rotate Array](https://leetcode.com/problems/rotate-array/) | Medium | [Arrays, Hashing & Prefix Sums](01-arrays-and-hashing.md) |
| 253 | [Meeting Rooms II](https://leetcode.com/problems/meeting-rooms-ii/) 🔒 | Medium | [Intervals & Sweep Line](10-intervals-and-sweep-line.md) |

---

## Suggested 12-week plan (about 2 hours a day)

| Week | Topics | Goal |
|---|---|---|
| 1 | 01, 02, 03 | Hash maps, prefix sums, pointer and window templates |
| 2 | 04, 05 | Monotonic stack; binary search on the answer |
| 3 | 06, 07 | Pointer manipulation; know what every tree recursion returns |
| 4 | 08, 09, 10 | Tries, heap patterns, sweep line |
| 5 | 11, 12 | Greedy proofs; backtracking with pruning |
| 6 | 13 | BFS/DFS on grids and graphs, expanded states, topological sort |
| 7 | 14 | Dijkstra, 0-1 BFS, union-find, MST |
| 8–9 | 15 | DP: follow the schedule inside the [DP list](15-dynamic-programming.md) |
| 10 | 16, 17 | Bit tricks, overflow, randomization; KMP and rolling hash |
| 11 | 18, 19 | Design classes; Fenwick/segment trees and ordered sets |
| 12 | Hot lists + re-solve ⭐ | Timed mocks: 45 minutes for 1 Medium + 1 follow-up, or 1 Hard |

**Ready for interviews when:** you can solve an unseen ⭐-level Medium in under 25 minutes and a Hard in under 40 minutes, while explaining your approach, complexity, and tests out loud.

---

## What the interview expects

Use the same steps for every problem, including practice:

1. **Clarify:** input sizes, value ranges, duplicates, empty input, and the exact output format.
2. **Examples:** work through one normal case and one edge case by hand before designing anything.
3. **Brute force first:** state it and its complexity in one sentence, then optimise.
4. **Name the pattern** and the invariant ("window is always valid", "stack is decreasing").
5. **Code cleanly:** use helper functions and meaningful names, with no unexplained magic indices.
6. **Test by hand:** trace your example through the code, then check edge cases.
7. **Complexity and follow-ups:** time, space, and what changes for streams, huge inputs, or concurrency.

**Company notes**
- **Google:** you usually code in a shared document with no compiler, so testing by hand and explaining correctness matter. Expect a follow-up that changes a constraint (larger input, streaming data, an O(1)-space version).
- **Uber:** coding rounds often use a runnable editor and multi-part problems that build on one another. Graph/routing problems (BFS, union-find, Dijkstra) and data-structure design classes appear far more often than in a typical company tag.

---

## How problems were chosen

- **Company-tag frequency data:** the Google (≈2,300 problems) and Uber (≈340 problems) LeetCode company tags, with all-time, 6-month, and 3-month frequency scores from the community-maintained [leetcode-company-wise-problems](https://github.com/liquidslr/leetcode-company-wise-problems) dataset (snapshot of 20 Oct 2026).
- **Ranking:** within each LeetCode topic, problems were ranked by combined Google and Uber frequency, with extra weight for recent Uber appearances. Top-ranked problems were kept when they teach a distinct pattern; near-duplicates were dropped.
- **Pattern coverage:** a few untagged classics were added where a pattern needed a clean teaching problem (shown as `—`).
- **Accuracy:** every problem number, title, link, difficulty, and Premium flag was checked against the official LeetCode catalogue (4,096 problems). No problem appears in more than one file; the "See also" notes point to related problems in other topic files.
- **Limitations:** company tags come from candidate reports and change constantly. Treat the tags as a guide to what to practise, not a prediction of your exact questions.

---

## Extra resources

- [LeetCode company tags: Google](https://leetcode.com/company/google/) and [Uber](https://leetcode.com/company/uber/) (Premium): the live source for the `G`/`U` signals
- [NeetCode roadmap](https://neetcode.io/roadmap): a dependency graph of the same core patterns, with video solutions
- [Tech Interview Handbook: algorithms study cheatsheets](https://www.techinterviewhandbook.org/algorithms/study-cheatsheet/): edge cases and techniques per topic
- [CSES Problem Set](https://cses.fi/problemset/): short, standard problems for graphs, range queries, and DP
