# DP Interview Practice: Google / Uber Level

⭐ = must do. If short on time, do only the 38 ⭐ problems.
For every problem: **Recursion → Memoization → Bottom-up → Space optimized.**

---

## How to practice each problem

1. Write `f(state)` as one sentence.
2. List the choices at the current element. Each choice = gain now + f(what's left).
3. Combine: max/min → `max`/`min`, count → `+`, possible? → `||`.
4. Memoize: pure function, no `void` + global, remove "so far" params, keys must be small integers.
5. Convert to bottom-up: params → dimensions, base cases → starting values, `f(i+1)` → loop `i` downward.
6. Explain the time and space complexity out loud, as you would in an interview.
7. Re-solve ⭐ problems after 7 days without looking at your old code.

---

## 1. Linear DP (1D)

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ✅ | 70 | [Climbing Stairs](https://leetcode.com/problems/climbing-stairs/) | Easy | warm-up |
| ✅ | 198 | [House Robber](https://leetcode.com/problems/house-robber/) ⭐ | Medium | take or skip |
| ✅ | 213 | [House Robber II](https://leetcode.com/problems/house-robber-ii/) | Medium | circular → 2 linear runs |
| ✅ | 740 | [Delete and Earn](https://leetcode.com/problems/delete-and-earn/) | Medium | reduce to House Robber |
| ✅ | 91 | [Decode Ways](https://leetcode.com/problems/decode-ways/) ⭐ | Medium | count ways |
| ✅ | 139 | [Word Break](https://leetcode.com/problems/word-break/) ⭐ | Medium | where the first piece ends |
| ✅ | 322 | [Coin Change](https://leetcode.com/problems/coin-change/) ⭐ | Medium | choose one of k options |
| ☐ | 279 | [Perfect Squares](https://leetcode.com/problems/perfect-squares/) | Medium | choose one of k options |
| ☐ | 983 | [Minimum Cost For Tickets](https://leetcode.com/problems/minimum-cost-for-tickets/) | Medium | choose one of k options |
| ☐ | 2140 | [Solving Questions With Brainpower](https://leetcode.com/problems/solving-questions-with-brainpower/) | Medium | take or skip with jump |
| ✅ | 152 | [Maximum Product Subarray](https://leetcode.com/problems/maximum-product-subarray/) ⭐ | Medium | track max and min |

## 2. Grid DP

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 62 | [Unique Paths](https://leetcode.com/problems/unique-paths/) | Medium | grid count |
| ☐ | 63 | [Unique Paths II](https://leetcode.com/problems/unique-paths-ii/) | Medium | grid with obstacles |
| ☐ | 64 | [Minimum Path Sum](https://leetcode.com/problems/minimum-path-sum/) | Medium | grid min |
| ☐ | 120 | [Triangle](https://leetcode.com/problems/triangle/) | Medium | variable-width grid |
| ☐ | 931 | [Minimum Falling Path Sum](https://leetcode.com/problems/minimum-falling-path-sum/) | Medium | 3 moves |
| ✅ | 221 | [Maximal Square](https://leetcode.com/problems/maximal-square/) ⭐ | Medium | min of 3 neighbours |
| ✅ | 174 | [Dungeon Game](https://leetcode.com/problems/dungeon-game/) ⭐ | Hard | must fill from the bottom-right |
| ✅ | 1463 | [Cherry Pickup II](https://leetcode.com/problems/cherry-pickup-ii/) ⭐ | Hard | 2 walkers → 3D state |
| ☐ | 741 | [Cherry Pickup](https://leetcode.com/problems/cherry-pickup/) | Hard | 2 walkers, same step count |
| ☐ | 329 | [Longest Increasing Path in a Matrix](https://leetcode.com/problems/longest-increasing-path-in-a-matrix/) ⭐ | Hard | DFS + memo on a grid |

## 3. Knapsack (Subset / Target)

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ✅ | 416 | [Partition Equal Subset Sum](https://leetcode.com/problems/partition-equal-subset-sum/) ⭐ | Medium | 0/1 knapsack, bool |
| ✅ | 494 | [Target Sum](https://leetcode.com/problems/target-sum/) ⭐ | Medium | 0/1 knapsack, count, offset |
| ☐ | 1049 | [Last Stone Weight II](https://leetcode.com/problems/last-stone-weight-ii/) | Medium | reduce to subset sum |
| ✅ | 518 | [Coin Change II](https://leetcode.com/problems/coin-change-ii/) ⭐ | Medium | unbounded, combinations |
| ☐ | 377 | [Combination Sum IV](https://leetcode.com/problems/combination-sum-iv/) | Medium | unbounded, permutations (compare with 518) |
| ☐ | 474 | [Ones and Zeroes](https://leetcode.com/problems/ones-and-zeroes/) | Medium | 2 capacities |
| ☐ | 879 | [Profitable Schemes](https://leetcode.com/problems/profitable-schemes/) | Hard | capped dimension |
| ☐ | 956 | [Tallest Billboard](https://leetcode.com/problems/tallest-billboard/) | Hard | difference as the state |

## 4. Two Strings / Sequences

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 1143 | [Longest Common Subsequence](https://leetcode.com/problems/longest-common-subsequence/) ⭐ | Medium | match or skip |
| ☐ | 583 | [Delete Operation for Two Strings](https://leetcode.com/problems/delete-operation-for-two-strings/) | Medium | LCS variant |
| ☐ | 712 | [Minimum ASCII Delete Sum for Two Strings](https://leetcode.com/problems/minimum-ascii-delete-sum-for-two-strings/) | Medium | weighted LCS |
| ☐ | 72 | [Edit Distance](https://leetcode.com/problems/edit-distance/) ⭐ | Medium | insert / delete / replace |
| ☐ | 97 | [Interleaving String](https://leetcode.com/problems/interleaving-string/) ⭐ | Medium | 2 pointers → 2D state |
| ☐ | 115 | [Distinct Subsequences](https://leetcode.com/problems/distinct-subsequences/) | Hard | count matches |
| ☐ | 1092 | [Shortest Common Supersequence](https://leetcode.com/problems/shortest-common-supersequence/) | Hard | rebuild the answer from the table |
| ☐ | 10 | [Regular Expression Matching](https://leetcode.com/problems/regular-expression-matching/) ⭐ | Hard | `*` = zero or more |
| ☐ | 44 | [Wildcard Matching](https://leetcode.com/problems/wildcard-matching/) | Hard | `*` = any sequence |

## 5. Palindromes / Interval DP

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 5 | [Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/) | Medium | `isPal[i][j]` |
| ☐ | 647 | [Palindromic Substrings](https://leetcode.com/problems/palindromic-substrings/) | Medium | count `isPal` |
| ☐ | 516 | [Longest Palindromic Subsequence](https://leetcode.com/problems/longest-palindromic-subsequence/) ⭐ | Medium | i down, j up |
| ☐ | 1312 | [Minimum Insertion Steps to Make a String Palindrome](https://leetcode.com/problems/minimum-insertion-steps-to-make-a-string-palindrome/) | Hard | n − LPS |
| ☐ | 132 | [Palindrome Partitioning II](https://leetcode.com/problems/palindrome-partitioning-ii/) ⭐ | Hard | where the first piece ends |
| ☐ | 877 | [Stone Game](https://leetcode.com/problems/stone-game/) | Medium | game: my score − opponent's |
| ☐ | 1140 | [Stone Game II](https://leetcode.com/problems/stone-game-ii/) | Medium | game + extra param |
| ☐ | 375 | [Guess Number Higher or Lower II](https://leetcode.com/problems/guess-number-higher-or-lower-ii/) | Medium | minimax interval |
| ☐ | 1039 | [Minimum Score Triangulation of Polygon](https://leetcode.com/problems/minimum-score-triangulation-of-polygon/) | Medium | interval split |
| ☐ | 1547 | [Minimum Cost to Cut a Stick](https://leetcode.com/problems/minimum-cost-to-cut-a-stick/) ⭐ | Hard | interval split on cuts |
| ☐ | 312 | [Burst Balloons](https://leetcode.com/problems/burst-balloons/) ⭐ | Hard | pick the LAST balloon to burst |
| ☐ | 664 | [Strange Printer](https://leetcode.com/problems/strange-printer/) | Hard | interval merge |
| ☐ | 1000 | [Minimum Cost to Merge Stones](https://leetcode.com/problems/minimum-cost-to-merge-stones/) | Hard | interval + piles count |

## 6. LIS (Longest Increasing Subsequence)

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 300 | [Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence/) ⭐ | Medium | O(n²) DP, then O(n log n) |
| ☐ | 673 | [Number of Longest Increasing Subsequence](https://leetcode.com/problems/number-of-longest-increasing-subsequence/) | Medium | length + count |
| ☐ | 368 | [Largest Divisible Subset](https://leetcode.com/problems/largest-divisible-subset/) | Medium | LIS + print the path |
| ☐ | 1048 | [Longest String Chain](https://leetcode.com/problems/longest-string-chain/) ⭐ | Medium | LIS on strings |
| ☐ | 1626 | [Best Team With No Conflicts](https://leetcode.com/problems/best-team-with-no-conflicts/) | Medium | sort + LIS |
| ☐ | 354 | [Russian Doll Envelopes](https://leetcode.com/problems/russian-doll-envelopes/) | Hard | sort trick + LIS in n log n |

## 7. State Machine (Stocks)

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 121 | [Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/) | Easy | warm-up |
| ☐ | 122 | [Best Time to Buy and Sell Stock II](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/) | Medium | `holding` flag |
| ☐ | 309 | [Best Time to Buy and Sell Stock with Cooldown](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/) ⭐ | Medium | sell → i+2 |
| ☐ | 714 | [Best Time to Buy and Sell Stock with Transaction Fee](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/) | Medium | fee on sell |
| ☐ | 123 | [Best Time to Buy and Sell Stock III](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/) | Hard | transactions-left param |
| ☐ | 188 | [Best Time to Buy and Sell Stock IV](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/) ⭐ | Hard | general k |

## 8. Tree DP

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 543 | [Diameter of Binary Tree](https://leetcode.com/problems/diameter-of-binary-tree/) | Easy | return height, update answer |
| ☐ | 337 | [House Robber III](https://leetcode.com/problems/house-robber-iii/) ⭐ | Medium | return {take, skip} pair |
| ☐ | 124 | [Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/) ⭐ | Hard | path through the node vs path going up |
| ☐ | 968 | [Binary Tree Cameras](https://leetcode.com/problems/binary-tree-cameras/) ⭐ | Hard | 3 states per node |
| ☐ | 2246 | [Longest Path With Different Adjacent Characters](https://leetcode.com/problems/longest-path-with-different-adjacent-characters/) | Hard | top-2 children |
| ☐ | 834 | [Sum of Distances in Tree](https://leetcode.com/problems/sum-of-distances-in-tree/) | Hard | rerooting |

## 9. DP on Graphs / DAGs

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 787 | [Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) ⭐ | Medium | (node, stops) state / Bellman-Ford |
| ☐ | 1976 | [Number of Ways to Arrive at Destination](https://leetcode.com/problems/number-of-ways-to-arrive-at-destination/) | Medium | Dijkstra + count |
| ☐ | 2328 | [Number of Increasing Paths in a Grid](https://leetcode.com/problems/number-of-increasing-paths-in-a-grid/) | Hard | DFS + memo count |
| ☐ | 1857 | [Largest Color Value in a Directed Graph](https://leetcode.com/problems/largest-color-value-in-a-directed-graph/) | Hard | topological sort + DP |

## 10. Counting / Probability

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 935 | [Knight Dialer](https://leetcode.com/problems/knight-dialer/) ⭐ | Medium | well-known Google question |
| ☐ | 790 | [Domino and Tromino Tiling](https://leetcode.com/problems/domino-and-tromino-tiling/) | Medium | tiling states |
| ☐ | 576 | [Out of Boundary Paths](https://leetcode.com/problems/out-of-boundary-paths/) | Medium | (r, c, moves) |
| ☐ | 688 | [Knight Probability in Chessboard](https://leetcode.com/problems/knight-probability-in-chessboard/) ⭐ | Medium | probability DP |
| ☐ | 837 | [New 21 Game](https://leetcode.com/problems/new-21-game/) | Medium | probability + sliding window |
| ☐ | 808 | [Soup Servings](https://leetcode.com/problems/soup-servings/) | Medium | probability + cutoff |
| ☐ | 1220 | [Count Vowels Permutation](https://leetcode.com/problems/count-vowels-permutation/) | Hard | state transitions |
| ☐ | 920 | [Number of Music Playlists](https://leetcode.com/problems/number-of-music-playlists/) | Hard | combinatorial DP |

## 11. Bitmask DP

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 526 | [Beautiful Arrangement](https://leetcode.com/problems/beautiful-arrangement/) | Medium | mask = numbers used |
| ☐ | 698 | [Partition to K Equal Sum Subsets](https://leetcode.com/problems/partition-to-k-equal-sum-subsets/) ⭐ | Medium | mask + current sum |
| ☐ | 1986 | [Minimum Number of Work Sessions to Finish the Tasks](https://leetcode.com/problems/minimum-number-of-work-sessions-to-finish-the-tasks/) | Medium | mask + time left |
| ☐ | 847 | [Shortest Path Visiting All Nodes](https://leetcode.com/problems/shortest-path-visiting-all-nodes/) ⭐ | Hard | BFS on (node, mask) |
| ☐ | 1125 | [Smallest Sufficient Team](https://leetcode.com/problems/smallest-sufficient-team/) | Hard | mask = skills covered |
| ☐ | 1434 | [Number of Ways to Wear Different Hats to Each Other](https://leetcode.com/problems/number-of-ways-to-wear-different-hats-to-each-other/) | Hard | loop over hats, mask over people |
| ☐ | 943 | [Find the Shortest Superstring](https://leetcode.com/problems/find-the-shortest-superstring/) | Hard | travelling-salesman style |

## 12. Digit DP

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 902 | [Numbers At Most N Given Digit Set](https://leetcode.com/problems/numbers-at-most-n-given-digit-set/) | Hard | intro: (pos, tight) |
| ☐ | 233 | [Number of Digit One](https://leetcode.com/problems/number-of-digit-one/) ⭐ | Hard | (pos, count, tight) |
| ☐ | 600 | [Non-negative Integers without Consecutive Ones](https://leetcode.com/problems/non-negative-integers-without-consecutive-ones/) | Hard | (pos, prev bit, tight) |
| ☐ | 2376 | [Count Special Integers](https://leetcode.com/problems/count-special-integers/) | Hard | (pos, mask, tight, started) |
| ☐ | 1012 | [Numbers With Repeated Digits](https://leetcode.com/problems/numbers-with-repeated-digits/) | Hard | complement of 2376 |

## 13. Interview-Level Mixed Hard (final round)

| Done | # | Problem | Level | Pattern |
|---|---|---|---|---|
| ☐ | 2008 | [Maximum Earnings From Taxi](https://leetcode.com/problems/maximum-earnings-from-taxi/) ⭐ | Medium | ride-hailing theme; sort + DP |
| ☐ | 1235 | [Maximum Profit in Job Scheduling](https://leetcode.com/problems/maximum-profit-in-job-scheduling/) ⭐ | Hard | DP + binary search |
| ☐ | 1335 | [Minimum Difficulty of a Job Schedule](https://leetcode.com/problems/minimum-difficulty-of-a-job-schedule/) ⭐ | Hard | (i, days left) partition |
| ☐ | 410 | [Split Array Largest Sum](https://leetcode.com/problems/split-array-largest-sum/) ⭐ | Hard | partition DP (also binary search) |
| ☐ | 403 | [Frog Jump](https://leetcode.com/problems/frog-jump/) ⭐ | Hard | (stone, last jump) |
| ☐ | 32 | [Longest Valid Parentheses](https://leetcode.com/problems/longest-valid-parentheses/) | Hard | dp[i] = longest ending at i |
| ☐ | 85 | [Maximal Rectangle](https://leetcode.com/problems/maximal-rectangle/) | Hard | histogram per row |
| ☐ | 1406 | [Stone Game III](https://leetcode.com/problems/stone-game-iii/) | Hard | game DP |
| ☐ | 691 | [Stickers to Spell Word](https://leetcode.com/problems/stickers-to-spell-word/) | Hard | memo on remaining string |
| ☐ | 2218 | [Maximum Value of K Coins From Piles](https://leetcode.com/problems/maximum-value-of-k-coins-from-piles/) | Hard | group knapsack |
| ☐ | 818 | [Race Car](https://leetcode.com/problems/race-car/) | Hard | BFS / DP on position |

---

## Suggested 6-week schedule (about 2 hours a day)

| Week | Sections | Goal |
|---|---|---|
| 1 | 1, 2 | Recursion → memo → table on every problem |
| 2 | 3, 4 | Knapsack loop order; two-string tables |
| 3 | 5, 6 | Interval fill order; LIS in n log n |
| 4 | 7, 8, 9 | Adding flags to the state; returning pairs from trees |
| 5 | 10, 11, 12 | Probability, bitmask, digit DP templates |
| 6 | 13 + re-solve all ⭐ | Timed: 35 minutes per problem, explain out loud |

**Ready for interviews when:** you can solve an unseen ⭐-level Medium in under 25 minutes and a Hard in under 40, and explain the state, transition and complexity without hesitating.

---

## Extra resources

- [LeetCode DP Study Plan](https://leetcode.com/studyplan/dynamic-programming/): structured set of 50 problems
- [Striver's DP Series (takeuforward)](https://takeuforward.org/dynamic-programming/striver-dp-series-dynamic-programming-problems): video for every problem, recursion → memo → table → space
- [AtCoder Educational DP Contest](https://atcoder.jp/contests/dp/tasks): 26 classic problems (A–Z), good for drilling patterns
- [CSES Problem Set: Dynamic Programming](https://cses.fi/problemset/): 19 short, standard DP problems
- [LeetCode DP patterns discussion](https://leetcode.com/discuss/study-guide/458695/Dynamic-Programming-Patterns): pattern-wise classification
