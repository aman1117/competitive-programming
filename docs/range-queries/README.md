# Range queries and offline processing

[Handbook](../README.md) | [Structures](../../include/cp/data_structures.hpp) | [Offline algorithms](../../include/cp/offline.hpp) | [Array helpers](../../include/cp/arrays.hpp)

**Prerequisites:** prefix sums, binary representation, and associative operations.  
**Goal:** choose the least complicated structure that supports the required update/query combination.

## Selection guide

| Workload | Start with |
|---|---|
| Static range sums | Prefix sums |
| Offline range additions, then read all values | Difference array |
| Point additions and range sums | Fenwick tree |
| Range additions and range sums | Two Fenwick trees or lazy sum tree |
| Point replacement and associative aggregation | Segment tree |
| Static range minimum with many queries | Sparse table |
| Static queries whose window state supports add/remove | Mo's algorithm |

## 1. Two-dimensional prefixes and difference arrays

`Prefix2D` stores a padded matrix of sums from the origin. A rectangle is obtained by subtracting the two unwanted strips and adding their overlap back.

For `[[1,2,3],[4,5,6]]`, the rectangle covering both rows and columns `[1,3)` sums to `2+3+5+6=16`.

A difference array reverses the prefix operation: adding `x` on `[l,r)` adds `x` at boundary `l` and `-x` at boundary `r`. A final prefix scan materializes all updates.

```cpp
cp::Prefix2D prefix({{1, 2, 3}, {4, 5, 6}});
assert(prefix.sum(0, 1, 2, 3) == 16);
auto values = cp::range_additions(4, {{0, 3, 2}, {1, 4, -1}});
assert((values == std::vector<cp::i64>{2, 1, 1, -1}));
```

**TC/SC:** 2D preprocessing takes `O((R+1)(C+1))` time and storage; rectangle queries are `O(1)`. Difference updates plus materialization take `O(q+n)` time and `O(n)` auxiliary space. Difference arrays do not directly answer interleaved online queries.

## 2. Coordinate compression

**Use when:** values are large but only their relative order matters.

Sort distinct values and replace each original value by its rank. For `[40,-2,40,7]`, sorted unique values are `[-2,7,40]`, giving ranks `[2,0,2,1]`. Equal values must receive the same rank.

```cpp
assert((cp::compress({40, -2, 40, 7}) == std::vector<int>{2, 0, 2, 1}));
```

**TC/SC:** `O(n log n)` time, `O(n)` workspace, plus rank output. Compression preserves ordering and equality, **not numeric distances**. When queries contain values absent from the array, use lower/upper bounds on the retained sorted coordinate list; this helper returns only ranks.

## 3. Fenwick tree: point additions and range sums

**Use when:** prefix aggregation has an inverse, such as sum subtraction.

The internal one-based bucket `i` covers `lowbit(i)=i&-i` elements ending at `i`. A prefix query repeatedly removes the lowest set bit, partitioning the prefix into disjoint buckets. A point update climbs to buckets containing that point.

For `[1,2,3,4,5]`, buckets are `[1,3,3,10,5]`. Prefix length five reads buckets `5` and `4`, giving `5+10=15`. Adding seven at zero-based index two changes internal buckets three and four.

```cpp
cp::Fenwick bit(std::vector<cp::i64>{1, 2, 3, 4, 5});
assert(bit.sum(0, 5) == 15);
bit.add(2, 7);
assert(bit.sum(1, 4) == 16);
```

**TC/SC:** the constructor uses a linear build, `O(n)`, rather than `n` logarithmic insertions. Updates and queries take `O(log n)` time with `O(1)` operation workspace. Stored space is `O(n)`. External indices remain zero-based.

### Prefix order statistics

If every point value is nonnegative, prefix sums are monotone. Fenwick binary lifting finds the first index whose inclusive prefix reaches a target.

```cpp
cp::Fenwick frequency(std::vector<cp::i64>{0, 2, 1, 3});
assert(frequency.lower_bound(3) == 2); // Prefixes: 0,2,3,6.
assert(frequency.lower_bound(7) == 4); // n: target exceeds total.
```

The search takes `O(log n)` time, not the `O(log^2 n)` of externally binary-searching repeated prefix queries. Negative point values invalidate this search even though ordinary sum queries still work.

## 4. Two Fenwick trees: range additions and sums

**Use when:** updates and queries both cover ranges, but the operation is specifically addition/sum.

Treat each range update as two changes to a difference array. A change `d` at index `i` contributes `d*(end-i)` to the sum of the first `end` values. Therefore:

```text
prefix_sum(end) = end * sum_of_deltas(end) - sum_of_index_times_delta(end)
```

Store the two terms in separate Fenwick trees.

```cpp
cp::RangeFenwick bit(4); // [0,0,0,0]
bit.add(1, 4, 3);        // [0,3,3,3]
bit.add(0, 2, 2);        // [2,5,3,3]
assert(bit.sum(0, 4) == 13);
assert(bit.sum(1, 3) == 8);
```

**TC/SC:** `O(n)` initialization and storage; each add/sum operation takes `O(log n)` time and `O(1)` workspace. The structure starts at zero. This derivation does not provide range assignment or arbitrary minimum queries.

## 5. Segment trees and monoids

**Use when:** range queries combine neighboring segments through an associative operation.

Each node stores the merge of its children. A query decomposes `[l,r)` into `O(log n)` relevant nodes. The merge must have an identity, such as zero for addition or `INF` for minimum.

Associativity allows regrouping. Commutativity is not required: the implementation maintains separate left and right accumulators to preserve order.

```cpp
auto minimum = [](cp::i64 x, cp::i64 y) { return std::min(x, y); };
cp::SegmentTree<cp::i64, decltype(minimum)> tree({5, 2, 7, 1}, cp::INF, minimum);
assert(tree.query(1, 3) == 2);
tree.set(1, 8);
assert(tree.query(1, 3) == 7);
assert(tree.query(2, 2) == cp::INF);
```

**TC/SC:** `O(n)` build/storage; point replacement and range query cost `O(log n)` time and `O(1)` operation workspace. These bounds assume constant-size values and constant-time merge. String concatenation is associative but has nonconstant cost.

## 6. Lazy propagation

**Use when:** a whole covered segment can be updated from its stored summary without visiting every leaf.

For range-add/range-sum, adding `d` to a segment of length `len` increases its stored sum by `d*len`. Store a pending tag instead of immediately descending. Before a partial query/update needs children, push the tag to them.

For `[1,2,3,4]`, adding two on `[1,4)` produces `[1,4,5,6]`. The fully covered right child changes from sum `7` to `11` in one step; the left side needs a partial descent.

```cpp
cp::LazySumTree tree({1, 2, 3, 4});
tree.add(1, 4, 2);
assert(tree.sum(0, 4) == 16);
assert(tree.sum(1, 3) == 9);
```

**TC/SC:** `O(n)` build and storage. Each operation takes `O(log n)` time and `O(log n)` recursion space because only boundary paths split. A query can mutate lazy bookkeeping while preserving logical values.

**Pitfall:** addition tags compose by addition. Assignment tags need a different composition rule and an explicit "pending assignment" state; this class does not implement assignment.

## 7. Sparse table

**Use when:** the array is immutable and the aggregation is idempotent, such as minimum.

Precompute every length-`2^k` block. For a query of length `L`, choose `k=floor(log2 L)` and combine two length-`2^k` blocks covering its ends. They may overlap; `min(x,x)=x` makes the overlap harmless.

For `[5,2,7,1,3]`, `[0,3)` uses blocks `[0,2)` and `[1,3)`, both with minimum `2`.

```cpp
cp::SparseMin table({5, 2, 7, 1, 3});
assert(table.query(0, 3) == 2);
assert(table.query(2, 5) == 1);
```

**TC/SC:** `O(n log n)` preprocessing time and storage, `O(1)` query time and workspace. Overlapping blocks cannot be used this way for sums, because overlap would be counted twice. Queries must be nonempty.

## 8. Mo's algorithm

**Use when:** all static range queries are known in advance and a current window can cheaply add/remove one element, but a simple associative merge is unavailable.

Sort queries by blocks of their left boundary, then by right boundary, reversing direction in alternating blocks. Move a single maintained window between queries instead of recomputing each answer.

For `[1,2,1,3]`, moving `[0,3)` to `[1,4)` removes one `1` but keeps another, then adds `3`; distinct count changes from two to three. The query `[2,2)` is empty and yields zero.

```cpp
auto answers = cp::mo_distinct({1, 2, 1, 3}, {{0, 3}, {1, 4}, {2, 2}});
assert((answers == std::vector<int>{2, 3, 0}));
```

**TC/SC:** compression costs `O(n log n)`, sorting queries `O(q log(q+1))`, and movement with square-root blocks costs `O((n+q)*sqrt(n+1))`. Space is `O(n+q)`. Within each left block the right pointer traverses the array; left changes are bounded by the block width per query.

This implementation uses compressed frequency arrays for `O(1)` add/remove. A map in the inner loop adds a logarithmic factor. It supports neither updates between queries nor online answers.
