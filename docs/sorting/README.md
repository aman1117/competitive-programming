# Merge sort and randomized quicksort

[Handbook](../README.md) | [Implementation](../../include/cp/sorting.hpp)

**Goal:** understand two fundamental sorting strategies and their different stability, memory, and worst-case guarantees. For ordinary contest sorting, prefer `std::sort` unless a problem requires a particular algorithm or stability.

## Merge sort

Split the array conceptually into runs, sort each run, and merge neighboring sorted runs. The implementation is bottom-up: run widths are `1,2,4,...`; no native recursion is used.

For `[4,1,3,2]`:

| Pass | Runs |
|---|---|
| Initial | `[4] [1] [3] [2]` |
| Width 1 | `[1,4] [2,3]` |
| Width 2 | `[1,2,3,4]` |

During a merge, the smallest remaining element must be one of the two run heads. Choose the left head on a tie, preserving the order of equivalent elements.

```cpp
std::vector<int> values{4, 1, 3, 2};
cp::merge_sort(values);
assert((values == std::vector<int>{1, 2, 3, 4}));

std::vector<std::pair<int, int>> records{{2, 0}, {1, 1}, {2, 2}};
cp::merge_sort(records, [](auto a, auto b) { return a.first < b.first; });
assert((records == std::vector<std::pair<int, int>>{{1, 1}, {2, 0}, {2, 2}}));
```

**TC:** `O(n log(n+1))` worst case: each pass processes every value and there are logarithmically many passes. **SC:** `O(n)` for a reusable merge buffer. The sort modifies the input and is **stable**. Values must be copyable; the comparator must provide a strict weak ordering. Costs assume constant-time comparisons/copies.

For inversion counting, use `count_inversions` in `search.hpp`; for a constant-extra-space linked-list variant, use `sort_list` in `nodes.hpp`.

## Randomized three-way quicksort

Choose a pivot and partition values into less-than, equivalent-to, and greater-than regions. Only the smaller and greater regions require further sorting. A large equal-key region is finished immediately, making all-equal input linear rather than repeatedly recursing on almost the entire array.

The partition maintains:

```text
[left, lower)     < pivot
[lower, current) == pivot
[current, upper)   unknown
[upper, right)    > pivot
```

After swapping a greater value with the last unknown position, do not advance `current`: the incoming value has not yet been classified.

```cpp
std::vector<int> values{3, 1, 3, 2, 3, 0};
cp::quick_sort(values, 42);
assert((values == std::vector<int>{0, 1, 2, 3, 3, 3}));
cp::quick_sort(values, 42, std::greater<int>{});
assert((values == std::vector<int>{3, 3, 3, 2, 1, 0}));
```

**TC:** expected `O(n log(n+1))` over random pivot choices, but worst-case `O(n^2)`. A fixed seed makes runs reproducible, **not adversarially safe**. **SC:** `O(log(n+1))` worst-case stack space: recurse only on the smaller partition and process the larger one iteratively, so each nested call is at most half as large.

The algorithm is **not stable**. It copies the pivot value so subsequent swaps cannot change the pivot accidentally. Values must be copyable and the comparator must be a strict weak ordering; never use `<=` as the comparator.

## Which should I use?

| Requirement | Choice |
|---|---|
| General contest sorting, worst-case `O(n log n)` comparison guarantee | `std::sort` |
| Stability with a standard-library implementation | `std::stable_sort` |
| Learning a stable array merge implementation | `cp::merge_sort` |
| Learning randomized partitioning and bounded recursion | `cp::quick_sort` |
| Count out-of-order pairs while sorting | `cp::count_inversions` |
| Sort linked nodes without an array buffer | `cp::sort_list` |

`std::sort` is not a promise of pure quicksort; standard-library implementations commonly combine strategies to retain the required comparison bound.
