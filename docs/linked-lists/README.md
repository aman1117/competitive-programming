# Linked-list algorithms

[Handbook](../README.md) | [Implementation](../../include/cp/nodes.hpp)

**Prerequisites:** pointers, object lifetime, and simple loop invariants.  
**Goal:** change links without losing nodes, creating unintended cycles, or hiding recursion space.

These helpers are **non-owning**: they do not allocate or delete nodes. Stack-allocated nodes in the examples remain alive for the entire snippet. On LeetCode use the provided `ListNode` definition rather than redeclaring it.

## 1. Reversing a list

**Use when:** the direction of an acyclic list must be reversed in place.

Maintain a reversed prefix headed by `previous` and an untouched suffix headed by `current`. Save `current->next` before redirecting that link; otherwise the remaining suffix is lost.

```cpp
cp::ListNode a(1), b(2), c(3);
a.next = &b; b.next = &c;
auto* head = cp::reverse_list(&a);
assert(head == &c && c.next == &b && b.next == &a && a.next == nullptr);
```

The reversed prefix grows from empty to `1`, then `2->1`, then `3->2->1`.

**TC/SC:** `O(n)` time and `O(1)` space. The original head becomes the tail. Empty input returns `nullptr`.

## 2. Slow/fast pointers: middle and cycle entry

For an acyclic list, advancing one pointer once and another twice locates the middle. The implementation returns the **second** middle for even length.

For cycle detection, the two pointers eventually meet inside a cycle because their relative position advances one step per iteration. Move one pointer back to the head, then advance both one step at a time; they meet at the cycle entry.

```cpp
cp::ListNode a(1), b(2), c(3), d(4);
a.next = &b; b.next = &c; c.next = &d;
assert(cp::middle_node(&a) == &c);
assert(cp::cycle_entry(&a) == nullptr);
d.next = &b;
assert(cp::cycle_entry(&a) == &b);
d.next = nullptr;
```

If the non-cyclic prefix has length `mu` and the cycle length is `lambda`, the meeting relation makes the reset pointers equally far from the entry modulo `lambda`.

**TC/SC:** both operations take `O(n)` time and `O(1)` space over distinct reachable nodes. `middle_node` requires an acyclic list; only `cycle_entry` is intended to handle cyclic input.

## 3. Intersection by pointer identity

**Use when:** two acyclic lists may share a physical tail.

Walk two pointers; when one reaches the end, switch it to the other head. Both traverse the same total length, so any difference in prefix lengths is canceled. They meet at the first shared node or at null.

```cpp
cp::ListNode a(1), b(2), shared(3);
a.next = &shared; b.next = &shared;
assert(cp::list_intersection(&a, &b) == &shared);
assert(cp::list_intersection(&a, nullptr) == nullptr);
```

**TC/SC:** `O(n+m)` time, `O(1)` space. Equal values are not sufficient: intersection concerns the same node address. Cyclic lists require a different analysis.

## 4. Merging sorted lists

**Use when:** two sorted, disjoint lists should become one sorted list without allocating replacement nodes.

A dummy head removes the need to special-case the first output node. Repeatedly attach the smaller head and advance only that input. Once one input is exhausted, attach the other suffix.

```cpp
cp::ListNode a(1), b(3), c(2), d(4);
a.next = &b; c.next = &d;
auto* head = cp::merge_sorted_lists(&a, &c);
assert(head == &a && a.next == &c && c.next == &b && b.next == &d);
assert(d.next == nullptr);
```

**TC/SC:** `O(n+m)` time, `O(1)` space. Equal values from the first list are selected first, making the merge stable. The inputs must be **disjoint**; feeding shared tails into a relinking merge can create cycles.

## 5. Reversing complete k-groups

**Use when:** every full group of `k` nodes should be reversed while leaving a short suffix untouched.

First find the kth node. If it does not exist, stop without altering that suffix. Save the node after the group, reverse links up to that boundary, then connect the previous group to the new group head.

For `1->2->3->4->5` and `k=2`, the result is `2->1->4->3->5`.

```cpp
cp::ListNode a(1), b(2), c(3), d(4), e(5);
a.next = &b; b.next = &c; c.next = &d; d.next = &e;
auto* head = cp::reverse_k_group(&a, 2);
assert(head == &b && b.next == &a && a.next == &d);
assert(d.next == &c && c.next == &e && e.next == nullptr);
```

**TC/SC:** `O(n)` time and `O(1)` space. Looking ahead and reversing may each touch a node, but that is still a constant number of visits, not `O(nk)`. Require `k>=1` and acyclic input.

## 6. Palindrome checking with restoration

**Use when:** comparing both halves without allocating an array of all values.

Find the first half's end, reverse the second half, compare matching nodes, then reverse that half back. Restoration must happen on mismatch as well as success.

```cpp
cp::ListNode a(1), b(2), c(2), d(1);
a.next = &b; b.next = &c; c.next = &d;
assert(cp::palindrome_list(&a));
assert(a.next == &b && b.next == &c && c.next == &d && d.next == nullptr);
d.val = 9;
assert(!cp::palindrome_list(&a));
assert(a.next == &b && b.next == &c && c.next == &d && d.next == nullptr);
```

**TC/SC:** `O(n)` time, `O(1)` space. The helper temporarily mutates links but restores the original topology before returning. A premature return on the first unequal pair would violate that contract.

## 7. Bottom-up merge sort

**Use when:** sorting a list with guaranteed `O(n log n)` time and constant auxiliary storage.

Sort runs of width one, then two, then four, and so on. Split two adjacent runs, merge them with the sorted-list helper, and append the result to the current pass.

For `4->2->1->3`, the width-one pass produces runs `2->4` and `1->3`; the width-two pass merges them into `1->2->3->4`.

```cpp
cp::ListNode a(4), b(2), c(1), d(3);
a.next = &b; b.next = &c; c.next = &d;
auto* head = cp::sort_list(&a);
assert(head == &c && c.next == &b && b.next == &d && d.next == &a);
assert(a.next == nullptr);
```

**TC/SC:** each pass touches `O(n)` nodes and there are `O(log n)` passes. Space is `O(1)` because both the outer sort and inner merge are iterative. A recursive top-down implementation would normally use `O(log n)` stack space, so do not transfer this constant-space claim to it.
