# Heaps and data-structure design

[Handbook](../README.md) | [Structures](../../include/cp/data_structures.hpp) | [K-way merge](../../include/cp/arrays.hpp) | [Numeric helper](../../include/cp/common.hpp)

**Prerequisites:** heaps, balanced ordered containers, hash maps, and linked lists.  
**Goal:** choose an invariant that makes each requested operation inexpensive and account for the actual retained memory.

## 1. K-way merging

**Use when:** several already-sorted sequences must be combined.

Only the first unconsumed value from each sequence can be the next global minimum. Put those candidates in a min-heap. After extracting one, replace it with the next value from the same sequence.

```cpp
auto merged = cp::merge_k_sorted({{1, 4, 5}, {1, 3, 4}, {2, 6}});
assert((merged == std::vector<cp::i64>{1, 1, 2, 3, 4, 4, 5, 6}));
```

The initial candidates are `1,1,2`. Taking the first `1` exposes `4` from that sequence, but the other `1` still wins next.

**TC/SC:** `O(k+N log(k+1))` time for `N` items across `k` sequences, with `O(k)` heap space excluding output. Initial candidates are heapified in linear time. For sorted linked lists, the same pattern stores node pointers rather than array positions, but it must preserve node ownership and disjointness contracts.

## 2. Online medians with two heaps

**Use when:** values arrive over time and the median is needed after insertions.

Maintain a max-heap `low` for the smaller half and a min-heap `high` for the larger half. Every low value must be at most every high value. Keep `low` the same size as `high` or one larger.

For insertions `1,5,2,4`, medians are `1,3,2,3`. The middle pair after the fourth insertion is `2,4`.

```cpp
cp::MedianFinder median;
assert(!median.median());
median.insert(1); median.insert(5);
assert(median.median() == 3.0L);
median.insert(2);
assert(median.median() == 2.0L);
median.insert(4);
assert(median.median() == 3.0L);
```

**TC/SC:** `O(log n)` insertion, `O(1)` median lookup, `O(n)` retained storage. This insertion-only class does not support arbitrary deletions.

### Numerically safe averaging

For an even count, avoid both overflowing `a+b` and prematurely converting opposite extreme integers to a low-precision floating type. `mean_of_two` uses:

```text
a/2 + b/2 + (a%2 + b%2)/2.0
```

The integer halves are added before conversion.

```cpp
assert(cp::mean_of_two(std::numeric_limits<cp::i64>::min(),
                       std::numeric_limits<cp::i64>::max()) == -0.5L);
```

This takes `O(1)` time/space. The final answer still has the platform's ordinary floating-point precision; the helper prevents avoidable intermediate overflow and cancellation, not all rounding.

## 3. Sliding-window medians

**Use when:** insertions and expiration/removal are both required.

Use two multisets with the same partition and size invariants as the heaps. When the window moves, erase one occurrence of the outgoing value, insert the incoming value, and rebalance.

For `[1,3,-1,-3,5,3,6,7]` with width three, medians are `[1,-1,-1,3,5,6]`.

```cpp
auto medians = cp::sliding_median({1, 3, -1, -3, 5, 3, 6, 7}, 3);
assert((medians == std::vector<long double>{1, -1, -1, 3, 5, 6}));
```

**TC/SC:** `O(n log(k+1))` time and `O(k)` workspace, plus output. Erase by iterator to remove one duplicate, not every equal value. Unlike naive lazy-deletion heap schemes, this implementation actually removes expired entries, so retained storage does not drift toward `O(n)`.

## 4. Binary XOR trie

**Use when:** finding the stored unsigned integer that maximizes XOR with a query.

Process bits from most significant to least significant. If the opposite bit exists, choose it: setting bit `b` in the XOR is worth more than every lower bit combined. Otherwise take the same-bit branch.

```cpp
cp::XorTrie trie;
assert(!trie.max_xor(5));
for (std::uint32_t x : {3U, 10U, 5U, 25U, 2U, 8U}) trie.insert(x);
assert(trie.max_xor(5) == 28U); // 5 XOR 25.
```

**TC/SC:** with fixed width `B=32`, insertion is amortized `O(B)`, query is worst-case `O(B)`, and worst-case storage is `O(nB)`. The result is the maximum XOR **value**, not the stored key. Use unsigned values; signed shifts and comparisons can change the interpretation of the highest bit.

## 5. Minimum stack

**Use when:** stack operations must also report the minimum.

Store each value together with the minimum at the time it was pushed. Popping reveals the previous snapshot automatically, so duplicate minima need no special-case removal logic.

```cpp
cp::MinStack stack;
stack.push(3); stack.push(1); stack.push(2);
assert(stack.minimum() == 1);
assert(stack.pop() == 2 && stack.minimum() == 1);
assert(stack.pop() == 1 && stack.minimum() == 3);
```

**TC/SC:** amortized `O(1)` push and `O(1)` pop/top/minimum. Retained storage is `O(peak stack size)`, because vector capacity remains after pops. Empty queries return `nullopt`. Arithmetic encodings such as storing `2*x-min` need overflow analysis; the explicit-pair implementation avoids that issue.

## 6. LRU cache

**Use when:** a bounded cache must evict the least recently used key.

Combine a doubly linked list ordered by recency with a hash map from key to list iterator. A hit moves its node to the front using a constant-time splice. When inserting beyond capacity, remove the back node and its map entry.

For capacity two:

| Operation | Most-recent to least-recent keys |
|---|---|
| Put 1, then put 2 | `2,1` |
| Get 1 | `1,2` |
| Put 3 | `3,1`; key 2 is evicted |

```cpp
cp::LRUCache cache(2);
cache.put(1, 10); cache.put(2, 20);
assert(cache.get(1) == 10);
cache.put(3, 30);
assert(!cache.get(2));
cache.put(1, 15);
assert(cache.get(1) == 15);
```

**TC/SC:** expected amortized `O(1)` get/put, worst-case `O(capacity)` hash-table operations, and `O(capacity)` storage. Gets change recency. Zero capacity is valid and retains nothing.

The class disables copying/moving: naively copying the map would leave its iterators pointing into the old object's list. Data-structure invariants include ownership and iterator validity, not just the abstract cache policy.

## 7. Randomized set

**Use when:** insertion, deletion, and uniform random choice are all needed.

Keep values densely packed in a vector and store each value's index in a hash map. To delete a value, move the last vector element into its position and update that element's index. Then remove the last slot.

```cpp
cp::RandomizedSet values;
std::mt19937 generator(7);
assert(values.insert(10));
assert(!values.insert(10));
assert(values.insert(20));
assert(values.erase(10));
assert(values.sample(generator) == 20);
assert(values.erase(20) && !values.sample(generator));
```

**TC/SC:** insertion/removal are expected amortized `O(1)` but can be `O(n)` for reallocation/hash behavior. Sampling is expected `O(1)` using a uniform integer distribution over live indices. Storage is `O(peak set size)` because capacity/buckets may remain after deletions.

`rng()%size` can introduce modulo bias; the implementation uses `uniform_int_distribution`. Swapping during removal means insertion order is not preserved.
