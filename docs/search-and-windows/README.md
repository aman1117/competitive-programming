# Search, prefixes, and sliding windows

[Handbook](../README.md) | [Array source](../../include/cp/arrays.hpp) | [Pattern source](../../include/cp/patterns.hpp)

**Prerequisites:** arrays, sorted order, maps, and loop invariants.  
**Goal:** recognize when a problem can avoid enumerating every subarray or every possible answer.

## 1. Prefix sums and prefix-frequency counting

**Use when:** many range sums are needed, or subarrays must satisfy a sum/XOR condition.

Define `p[0]=0` and `p[i+1]=p[i]+a[i]`. Then `[l,r)` has sum `p[r]-p[l]`: everything before `l` cancels. To count sums equal to `target`, rearrange to `p[l]=p[r]-target`; a frequency map counts all earlier valid boundaries.

For `[1,2,1]`, prefixes are `[0,1,3,4]`. At prefix `3`, an earlier `0` gives `[1,2]`. At prefix `4`, an earlier `1` gives `[2,1]`. The answer is two. Seed frequency `0 -> 1` to count subarrays starting at index zero, and query **before** inserting the current prefix to avoid counting an empty subarray.

```cpp
auto p = cp::prefix_sums({2, -1, 3});
assert(p[3] - p[1] == 2); // [-1,3]
assert(cp::count_subarrays_sum({1, 2, 1}, 3) == 2);
assert(cp::count_subarrays_xor({1, 2, 3}, 3) == 2);
```

For XOR, replace subtraction by XOR: `p[l]^p[r]=target` implies `p[l]=p[r]^target`. The example counts `[1,2]` and `[3]`.

**TC/SC:** prefix construction is `O(n)` time and `O(n)` output, with `O(1)` additional memory; each range query is `O(1)`. Both counting routines use ordered maps: `O(n log n)` time and `O(n)` space. A hash-map variant would be expected linear, not worst-case linear. Negative values do not invalidate prefix identities.

## 2. Binary search on a monotone answer

**Use when:** you can efficiently decide whether a candidate answer is feasible, and feasibility changes in only one direction.

`first_true(lo,hi,predicate)` searches `[lo,hi)` for the first true value. It preserves the invariant that the answer lies in the remaining interval, with the original `hi` representing "none found."

For splitting `[7,2,5,10,8]` into two pieces, a limit of `15` needs three pieces: `[7,2,5]`, `[10]`, `[8]`. A limit of `18` permits `[7,2,5]` and `[10,8]`. Increasing the limit cannot require more pieces, so binary search is valid.

```cpp
assert(cp::first_true(0, 100, [](cp::i64 x) { return x * x >= 30; }) == 6);
assert(cp::split_min_largest_sum({7, 2, 5, 10, 8}, 2) == 18);
assert(cp::first_true(0, 5, [](cp::i64) { return false; }) == 5);
```

The splitting predicate greedily extends each piece until the next value would exceed the limit. Nonnegative values justify both greediness and splitting an at-most-`k` solution further into exactly `k` nonempty pieces.

**TC/SC:** generic search makes `O(log(hi-lo+1))` predicate calls and uses `O(1)` workspace beyond the predicate. Splitting performs an `O(n)` scan per candidate: `O(n log(sum+1))` time, `O(1)` space. Negative values break this particular feasibility argument; a predicate is not automatically monotone just because it returns a Boolean.

## 3. Rotated-array search

**Use when:** a sorted array has been rotated, and you need a value's index.

At least one side of the midpoint is sorted when the boundary values distinguish the sides. Check whether the target lies in that sorted interval, then discard the other half. When `a[l]==a[mid]==a[r]`, equal endpoints hide which side contains the rotation; shrink both boundaries.

For `[2,2,2,3,1,2]`, the target `1` is at index `4`. Duplicates can repeatedly force only a one-position reduction.

```cpp
assert(cp::search_rotated({2, 2, 2, 3, 1, 2}, 1) == 4);
assert(cp::search_rotated({2, 2, 2}, 9) == -1);
```

**TC/SC:** `O(log n)` time for distinct values, `O(n)` worst case with duplicates, and `O(1)` space. The routine returns any matching index, not necessarily the first occurrence.

## 4. Selecting from two sorted arrays

**Use when:** finding a kth element or median without merging the arrays.

Choose `i` elements from the first array and `j=k-i` from the second. A valid partition has every chosen element no greater than every unchosen element. Binary-search `i` in the smaller array.

For `a=[1,3]`, `b=[2,4,5]`, and `k=3`, choose `i=2`, `j=1`. The left partition is `{1,3,2}`, the right is `{4,5}`, and the answer is the largest left value, `3`.

```cpp
std::vector<cp::i64> a{1, 3}, b{2, 4, 5};
assert(cp::kth_two_sorted(a, b, 3) == 3);
auto left = cp::kth_two_sorted({1, 3}, {2, 4}, 2);
auto right = cp::kth_two_sorted({1, 3}, {2, 4}, 3);
assert(cp::mean_of_two(left, right) == 2.5L);
```

**TC/SC:** `O(log(min(n,m)+1))` time and `O(1)` space. Here `k` is **one-based**. Both arrays must be sorted, and at least one must supply the requested element. The median of the combined arrays is not generally the average of their individual medians.

## 5. Sorted two pointers and three-sum

**Use when:** pair/triple sums can exploit sorted order.

For a sorted pair search, if `a[l]+a[r]` is too small, increasing `l` is the only useful move; decreasing `r` would make the sum even smaller. Reverse the reasoning when the sum is too large.

Three-sum sorts a copy, fixes one value, and uses the same pair search on the suffix. Equal fixed values and equal pair endpoints are skipped so each value triple appears once.

```cpp
auto pair = cp::two_sum_sorted({1, 2, 4}, 6);
assert(pair && pair->first == 1 && pair->second == 2);
auto triples = cp::three_sum({-1, 0, 1, 2, -1, -4});
std::vector<std::array<cp::i64, 3>> expected{{-1, -1, 2}, {-1, 0, 1}};
assert(triples == expected);
```

For fixed `-1`, pairs `(-1,2)` and `(0,1)` complete zero-sum triples.

**TC/SC:** pair search is `O(n)` time, `O(1)` space. Three-sum is `O(n^2)` time and `O(n)` space for the sorted copy, excluding potentially quadratic output. Three-sum returns values, not original indices.

## 6. Ordinary sliding windows

**Use when:** extending or shrinking a window changes validity monotonically.

For minimum length with a positive target and nonnegative values, extend right until the sum is sufficient; then shrink left while it remains sufficient. For "at most k bad elements," extend right and shrink only when the bad-element budget is exceeded.

```cpp
assert(cp::min_length_nonnegative({2, 3, 1, 2, 4, 3}, 7) == 2); // [4,3]
assert(cp::longest_ones_after_flips({1, 0, 1, 1, 0}, 1) == 4);
assert(cp::longest_unique_substring("abba") == 2);
```

In `"abba"`, the second `b` moves the left boundary past the earlier `b`. The final `a` must **not** move the boundary backward; use `max(current_left,last[a]+1)`.

**TC/SC:** each boundary advances at most `n` times, so the first two routines take `O(n)` time and `O(1)` space. The byte-string uniqueness routine uses a 256-entry last-occurrence array: `O(n+256)` time and `O(256)` space. These are byte operations, not Unicode code-point operations.

## 7. Minimum covering window

**Use when:** a substring must contain all required characters with multiplicity.

Track how many required characters remain missing. Decrement a character's need as it enters; only a previously positive need decreases the missing count. Once missing reaches zero, shrink left until removing a character makes the window invalid.

For `"ADOBECODEBANC"` and `"ABC"`, `"ADOBEC"` is an early valid window. Continuing the scan eventually produces `"BANC"` at `[9,13)`, length four.

```cpp
std::string s = "ADOBECODEBANC";
auto range = cp::minimum_window(s, "ABC");
assert(range && range->first == 9 && range->second == 13);
assert(s.substr(range->first, range->second - range->first) == "BANC");
assert(!cp::minimum_window("AB", "AA"));
```

**TC/SC:** `O(n+m+256)` time and `O(256)` space. Repeated target characters require repeated supply: a set of required characters is insufficient. An empty target returns `[0,0)`; failure returns `nullopt`.

## 8. Exactly k distinct values

**Use when:** directly maintaining "exactly k" makes counting awkward.

Every subarray with at most `k-1` distinct values is also in the set with at most `k`. Subtracting these counts leaves exactly `k`. For an at-most window ending at `r`, every start between its valid left boundary and `r` works, contributing `r-left+1`.

For `[1,2,1,2,3]`, there are `12` subarrays with at most two distinct values and `5` with at most one. Therefore exactly two gives `7`.

```cpp
assert(cp::subarrays_exactly_k_distinct({1, 2, 1, 2, 3}, 2) == 7);
assert(cp::subarrays_exactly_k_distinct({1, 1}, 0) == 0);
```

**TC/SC:** this implementation uses `std::map`, so two linear-pointer scans cost `O(n log n)` time and `O(n)` space overall, not guaranteed `O(n)`.

## 9. Positive-product windows

**Use when:** counting subarrays whose product is strictly below a limit, and every value is a positive integer.

For `[10,5,2,6]` and limit `100`, valid subarrays ending at each position number `1,2,2,3`; the total is `8`. At the third position, product `100` is excluded because the inequality is strict.

```cpp
assert(cp::count_product_less({10, 5, 2, 6}, 100) == 8);
assert(cp::count_product_less({1, 1, 1}, 1) == 0);
```

The implementation checks `product > (limit-1)/next_value` **before multiplying**, and shrinks first. This avoids overflowing a product that would immediately be discarded.

**TC/SC:** `O(n)` time and `O(1)` space. Zero, negative, or fractional values require different reasoning; the positive-integer contract is essential.

## 10. Negative values: use a monotone prefix deque

**Use when:** finding the shortest nonempty subarray with sum at least a target, but values may be negative.

Ordinary shrinking fails on `[1,-1,3]` with target `3`: after reaching sum `3`, removing the first `1` makes the sum `2`, yet removing the following `-1` would expose the optimal `[3]`.

Maintain candidate prefix boundaries with strictly increasing prefix sums. A later boundary with a smaller or equal prefix dominates an earlier one: it produces a larger subarray sum and a shorter length for every future endpoint.

For prefixes `[0,1,0,3]`, prefix index `2` dominates indices `0` and `1`. At index `3`, subtracting prefix index `2` yields sum `3` and length `1`.

```cpp
assert(cp::shortest_subarray_at_least({1, -1, 3}, 3) == 1);
assert(cp::shortest_subarray_at_least({2, -1, 2}, 3) == 3);
assert(cp::shortest_subarray_at_least({-2, -1}, 5) == -1);
```

**TC/SC:** `O(n)` time because each prefix enters and leaves the deque at most once; `O(n)` space for prefixes and candidates. This is an optimization over prefix boundaries, not a conventional window that assumes nonnegative elements.
