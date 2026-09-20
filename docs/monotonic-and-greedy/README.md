# Monotonic structures and greedy algorithms

[Handbook](../README.md) | [Implementation](../../include/cp/arrays.hpp)

**Prerequisites:** sorted order, stacks, queues, and [window invariants](../search-and-windows/README.md).  
**Goal:** justify discarding candidates and making choices that do not need to be reconsidered.

## 1. Next greater elements

**Use when:** an element needs its first strictly larger value to the right.

Keep unresolved indices in nonincreasing value order. A new larger value resolves every smaller stack-top element: no earlier position could have resolved it, otherwise it would already have been popped.

For `[2,1,2,4,3]`, the second `2` resolves the `1`. The `4` resolves both earlier `2`s. Values `4` and `3` have no larger successor.

```cpp
auto next = cp::next_greater({2, 1, 2, 4, 3});
assert((next == std::vector<int>{3, 2, 3, 5, 5}));
```

**TC/SC:** `O(n)` time, `O(n)` stack space, plus output. Each index is pushed and popped once. Equality does not resolve a **strictly** greater query; changing `<` to `<=` changes the problem. The missing-index sentinel is `n`.

## 2. Sliding-window maximum

**Use when:** finding the maximum in every fixed-width window.

The deque stores live indices in decreasing value order. A newer value at least as large as an older one dominates it: the newer value is better and expires later. Remove expired indices from the front.

For `[1,3,-1,-3,5,3,6,7]` and width three, the first maximum is `3`. When `5` arrives, it removes all weaker trailing candidates; the next maximum becomes `5`.

```cpp
auto maximum = cp::sliding_max({1, 3, -1, -3, 5, 3, 6, 7}, 3);
assert((maximum == std::vector<cp::i64>{3, 3, 5, 5, 6, 7}));
```

**TC/SC:** `O(n)` time, `O(k)` auxiliary space, and `O(n-k+1)` output. Storing only values without enough information to expire duplicates is a common mistake.

## 3. Largest histogram rectangle and maximal matrix rectangle

**Use when:** a rectangle's area is determined by its minimum bar height.

Maintain increasing heights. When a lower bar arrives, each popped height has found its right boundary. The new stack top provides its left boundary; all bars strictly between those boundaries are tall enough.

In `[2,1,5,6,2,3]`, popping height `5` at index `2` discovers boundaries `1` and `4`. Width is `4-1-1=2`, so area is `5*2=10`.

```cpp
assert(cp::largest_rectangle({2, 1, 5, 6, 2, 3}) == 10);
assert(cp::maximal_rectangle({"10100", "10111", "11111", "10010"}) == 6);
```

For a binary matrix, accumulate consecutive-one heights by column. After the third example row, heights are `[3,1,3,2,2]`; the final three columns support a height-two rectangle of area six.

**TC/SC:** histogram processing is `O(n)` time and `O(n)` space. Processing each matrix row as a histogram takes `O(R(C+1))` time and `O(C)` space. The extra `+1` includes zero-width rows. Heights must be nonnegative; matrix entries must be `'0'` or `'1'`.

## 4. Sum of subarray minimums: contribution counting

**Use when:** summing a statistic over all subarrays is quadratic, but each element can be assigned responsibility for a family of subarrays.

If index `i` has previous strictly smaller boundary `l` and next smaller-or-equal boundary `r`, it owns `(i-l)*(r-i)` subarrays. Choose the left endpoint from `(l,i]` and the right endpoint from `[i,r)`.

For `[3,1,2,4]`:

| Value | Number of owned subarrays | Contribution |
|---|---:|---:|
| 3 | 1 | 3 |
| 1 | 6 | 6 |
| 2 | 2 | 4 |
| 4 | 1 | 4 |

The total is `17`. For `[2,2]`, the first value owns one subarray and the second owns two, giving `6`.

```cpp
assert(cp::sum_subarray_minimums({3, 1, 2, 4}) == 17);
assert(cp::sum_subarray_minimums({2, 2}) == 6);
```

**TC/SC:** `O(n)` time and `O(n)` stack space. One boundary must be strict and the other non-strict, or equal minima are double-counted or missed. This API returns a **modular** sum; exact range-sum problems require exact accumulation and a corresponding maximum-contribution calculation.

## 5. Trapping rain water

**Use when:** each position's capacity depends on the highest boundary to its left and right.

The water above a position is `min(left_max,right_max)-height`, when positive. A two-pointer scan processes the side with the smaller known boundary: that side can already be settled without knowing the entire interior.

For `[3,0,2,0,4]`, the three interior positions hold `3`, `1`, and `3` units, totaling `7`.

```cpp
assert(cp::trapped_water({3, 0, 2, 0, 4}) == 7);
```

**TC/SC:** `O(n)` time, `O(1)` space. Each pointer visits each position at most once. This differs from histogram rectangles: water uses two bounding maxima, whereas a rectangle is limited by a minimum.

## 6. Maximum sum, maximum product, and products excluding self

**Use when:** a left-to-right summary is enough to extend an optimal subarray or compute prefix/suffix contributions.

Kadane's state is the best sum **ending here**: either extend the previous subarray or start again. For `[4,-1,2,1]`, extending gives `6`; a negative earlier prefix can be discarded.

Products need both the largest and smallest ending products. On `[-2,3,-4]`, states after `3` include a minimum of `-6`; multiplying it by `-4` produces the maximum `24`.

```cpp
assert(cp::max_subarray_sum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
assert(cp::max_subarray_sum({-5, -2}) == -2);
assert(cp::max_product_subarray({-2, 3, -4}) == 24);
assert((cp::product_except_self({1, 2, 0, 4}) == std::vector<cp::i64>{0, 0, 8, 0}));
```

`product_except_self` first writes the product before each position, then multiplies by the suffix product after it. No division is needed, so zeros work naturally.

**TC/SC:** all three take `O(n)` time and `O(1)` auxiliary space, excluding the product-array output. The maximum-subarray routines require a nonempty answer and return `nullopt` on empty input. Every intermediate product must fit the integer type; a fitting final answer alone is insufficient.

## 7. Interval merging, interval scheduling, and room allocation

These look similar but answer different questions.

| Problem | Invariant | Example |
|---|---|---|
| Merge closed intervals | The last output interval is the union of all overlapping intervals seen so far | `[1,3],[2,6]` becomes `[1,6]` |
| Maximize nonoverlapping jobs | Choose the earliest finishing compatible job | `[1,3]` then `[3,5]` beats taking only `[2,4]` |
| Minimum meeting rooms | Track simultaneous active half-open intervals | `[0,10],[5,7],[10,12]` needs two rooms |

```cpp
auto merged = cp::merge_intervals({{1, 3}, {2, 6}, {8, 10}});
assert((merged == std::vector<std::pair<cp::i64, cp::i64>>{{1, 6}, {8, 10}}));
assert(cp::max_nonoverlapping({{1, 3}, {2, 4}, {3, 5}}) == 2);
assert(cp::meeting_rooms({{0, 10}, {5, 7}, {10, 12}}) == 2);
```

Why earliest finish is safe: replace the first job of an optimal solution by an earlier-finishing compatible job. It leaves at least as much room for the remaining jobs. This exchange argument does **not** justify ignoring profit; weighted scheduling needs DP.

**TC/SC:** `O(n log n)` time from sorting and `O(n)` workspace/copies. Room allocation sorts end events before start events at equal times. Merging uses closed intervals, while scheduling and room allocation use nonempty half-open intervals.

## 8. Greedy jump layers

**Use when:** each position permits jumping forward by up to a nonnegative distance.

Maintain the current layer's last reachable index and the farthest index reachable from that layer. When the scan reaches the layer boundary, commit one jump and start the next layer. This is BFS over intervals without materializing all jump edges.

For `[2,3,1,1,4]`, one jump reaches indices `1..2`. From that layer, index `1` can reach `4`, so the answer is two jumps.

```cpp
assert(cp::min_jumps({2, 3, 1, 1, 4}) == 2);
assert(cp::min_jumps({3, 2, 1, 0, 4}) == -1);
```

**TC/SC:** `O(n)` time, `O(1)` space. If the layer cannot extend, the last position is unreachable. Do not assume every input is solvable.

## 9. In-place placement, three-way partitioning, and majority voting

These are supporting invariant patterns rather than interchangeable sorting techniques.

**Cyclic placement:** for first missing positive, put in-range value `x` at index `x-1`. On `[3,4,-1,1]`, swaps reach `[1,-1,3,4]`; the first mismatch reveals `2`. Each productive swap permanently places a value. A duplicate already occupying its destination must stop the loop.

**Dutch flag:** maintain finished zero, one, and two regions around an unknown region. Swapping a `2` with the right boundary does not advance the middle pointer because the incoming value is still unclassified.

**Boyer-Moore:** cancel pairs of unequal values. A strict majority cannot be completely canceled, but a surviving candidate may still not be a majority; verify it.

```cpp
std::vector<int> values{3, 4, -1, 1};
assert(cp::first_missing_positive(values) == 2);
std::vector<int> colors{2, 0, 2, 1, 1, 0};
cp::sort_three_values(colors);
assert((colors == std::vector<int>{0, 0, 1, 1, 2, 2}));
assert(cp::majority_element({2, 2, 1, 1, 1, 2, 2}) == 2);
assert(!cp::majority_element({1, 2, 3}));
```

**TC/SC:** each routine takes `O(n)` time and `O(1)` space. Placement and Dutch flag mutate the input. Dutch flag accepts only `0/1/2`; voting returns `nullopt` when no value occurs more than `n/2` times.
