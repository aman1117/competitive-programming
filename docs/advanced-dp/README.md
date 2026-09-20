# Interval, bitmask, and digit dynamic programming

[Handbook](../README.md) | [Implementation](../../include/cp/dp.hpp)

**Prerequisites:** [state definitions and update order](../sequence-dp/README.md), bitmasks, and combinatorial counting.  
**Goal:** turn seemingly global decisions into bounded states with explicit transitions.

## 1. Matrix-chain multiplication

**Use when:** a sequence of matrix products must be parenthesized to minimize scalar multiplications.

Matrix `i` has dimensions `d[i] x d[i+1]`. Let `dp[l][r]` be the minimum cost to multiply matrices `l..r`. Try every split `k`:

```text
dp[l][r] = min(dp[l][k] + dp[k+1][r] + d[l]*d[k+1]*d[r+1])
```

For dimensions `[10,30,5,60]`, `(AB)C` costs `1500+3000=4500`; `A(BC)` costs `9000+18000=27000`.

```cpp
assert(cp::matrix_chain({10, 30, 5, 60}) == 4500);
```

**Why it works:** the last multiplication splits the chain into two independent contiguous subchains. Enumerating every possible last split includes the optimal parenthesization.

**TC/SC:** `O(n^3)` time: `O(n^2)` intervals each try `O(n)` splits. Space is `O(n^2)`. All dimension products and accumulated costs must fit below `INF`.

## 2. Burst Balloons: choose the last removal

**Use when:** removing an item changes its neighbors, making a "first choice" difficult to separate into independent subproblems.

Pad the array with boundary ones. For open interval `(l,r)`, choose which balloon is removed **last**. Its neighbors are then fixed at `l` and `r`, so:

```text
dp[l][r] = max(dp[l][last] + dp[last][r] + a[l]*a[last]*a[r])
```

For `[3,1,5,8]`, removal order `1,5,3,8` earns `15+120+24+8=167`.

```cpp
assert(cp::burst_balloons({3, 1, 5, 8}) == 167);
assert(cp::burst_balloons({}) == 0);
```

**TC/SC:** `O(n^3)` time and `O(n^2)` space. The general lesson is to reverse the decision perspective when earlier removals change later boundaries. Values must be nonnegative in this template.

## 3. Palindrome minimum cuts

**Use when:** splitting a string into the fewest palindromic pieces.

Precompute [palindrome intervals](../strings/README.md). Let `pieces[r]` be the minimum number of pieces covering prefix `[0,r)`. For each palindromic suffix `[l,r)`, try `pieces[l]+1`.

For `"aab"`, prefix piece counts are `[0,1,1,2]`: `"a"`, `"aa"`, then `"aa"|"b"`. Two pieces need one cut.

```cpp
assert(cp::palindrome_min_cuts("aab") == 1);
assert(cp::palindrome_min_cuts("") == 0);
```

**TC/SC:** `O(n^2)` time and space, dominated by the palindrome table. Minimum cuts and enumeration are different problems: this returns one optimal count, while [partition backtracking](../backtracking/README.md) may emit exponentially many answers.

## 4. Two-player interval games

**Use when:** players optimally take one value from either end.

Store the best **score difference for the current player**, not two separate accumulated scores. Taking the left value gives `a[l] - opponent_advantage_on_the_rest`; subtracting flips the perspective.

```text
dp[l][r] = max(a[l]-dp[l+1][r], a[r]-dp[l][r-1])
```

For `[1,5,2]`, the first player can collect only three while the second collects five, so the difference is `-2`.

```cpp
assert(cp::take_ends_score_difference({1, 5, 2}) == -2);
```

**TC/SC:** `O(n^2)` time and `O(n)` space using a rolling interval row. A negative result means the first player loses under optimal play. Other game rules, such as taking multiple items or changing move limits, need additional state.

## 5. Held-Karp traveling-salesman DP

**Use when:** visiting each vertex exactly once and returning to a fixed start, with a small vertex count.

`dp[mask][v]` is the cheapest path starting at zero, visiting exactly `mask`, and ending at `v`. Add an unvisited vertex `u`, then finally add the edge returning to zero.

For the matrix below, route `0->1->3->2->0` costs `10+25+30+15=80`.

```cpp
cp::Matrix cost{{0, 10, 15, 20}, {10, 0, 35, 25},
                {15, 35, 0, 30}, {20, 25, 30, 0}};
assert(cp::traveling_salesman(cost) == 80);
assert(!cp::traveling_salesman({{0, 2}, {cp::INF, 0}}));
```

That route visits states `(0001,0)`, `(0011,1)`, `(1011,3)`, `(1111,2)` before closing the tour.

**TC/SC:** `O(n*2^n)` states, each considering up to `n` next vertices: `O(n^2*2^n)` time and `O(n*2^n)` space. At `n=20`, the `i64` table alone is about 160 MiB. `INF` denotes missing edges; `nullopt` means no tour. Unlike [visit-all BFS](../graph-paths/README.md), this does not allow repeated vertices.

## 6. SOS DP / subset zeta transform

**Use when:** every mask needs the sum of a function over all its submasks.

Initially `dp[mask]=f[mask]`. For each bit, if that bit is set in `mask`, add `dp[mask without bit]`. After processing a set of bits, all choices of deleting those bits have been accounted for.

For two bits and values `[1,2,3,4]`:

| Stage | `00` | `01` | `10` | `11` |
|---|---:|---:|---:|---:|
| Initial | 1 | 2 | 3 | 4 |
| Process bit 0 | 1 | 3 | 3 | 7 |
| Process bit 1 | 1 | 3 | 4 | 10 |

```cpp
assert((cp::subset_zeta({1, 2, 3, 4}) == std::vector<cp::i64>{1, 3, 4, 10}));
```

**TC/SC:** `O(B*2^B)` time, with an `O(2^B)` by-value working/result vector and `O(1)` additional workspace. Enumerating all submasks for every mask costs `O(3^B)`: each bit is absent, present only in the outer mask, or present in both. Input length must be exactly `2^B`.

## 7. Digit DP for a target digit sum

**Use when:** a numeric bound is huge but its decimal representation is short.

The state is `(position,sum,tight)`. `tight` means the prefix still equals the bound's prefix, so the next digit cannot exceed the corresponding bound digit. Once a smaller digit is chosen, remaining digits may range from zero through nine.

For `bound=25` and sum three, valid numbers are `03`, `12`, and `21`, interpreted as `3,12,21`. Leading zero padding does not alter the sum.

```cpp
assert(cp::count_digit_sum(25, 3) == 3);
assert(cp::count_digit_sum(25, 0) == 1); // Includes zero.
assert(cp::count_digit_sum(25, 3) - cp::count_digit_sum(9, 3) == 2); // [10,25].
```

**TC/SC:** `D*(S+1)*2` states per full position-expanded view, up to ten digit transitions each: `O(D*(S+1)*10)` time. Rolling positions use `O(S+1)` space. Bounds are limited to `10^18`; counts fit `i64` under this contract.

`count_digit_sum(-1,S)=0` makes nonnegative range subtraction convenient. Constraints involving actual digits, not just their sum, often need a separate started state.

## 8. Digit DP with a used-digit mask

**Use when:** digit uniqueness or other digit-set constraints matter.

Track which of the ten digits have appeared. The all-zero mask also means the number has not started. A leading zero keeps mask zero; after starting, digit zero is a real digit and occupies its bit.

Among positive integers through 25, only `11` and `22` repeat a digit, so the answer is `23`.

```cpp
assert(cp::count_unique_digit_numbers(25) == 23);
assert(cp::count_unique_digit_numbers(100) == 90);
assert(cp::count_unique_digit_numbers(0) == 0);
```

If padding consumed zero immediately, valid numbers containing a later real zero, such as `10`, would be wrongly rejected.

**TC/SC:** `O(D*2^10*10)` time and `O(2^10)` rolling-layer space. This function counts **positive** numbers, unlike the digit-sum function's inclusion of zero. A remainder condition adds another state dimension; account for it in both time and memory rather than reusing the same complexity claim.
