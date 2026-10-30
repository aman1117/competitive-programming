# Stacks & Monotonic Stack: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Monotonic structures and greedy](../docs/monotonic-and-greedy/README.md), [Heaps and data-structure design §5 (min stack)](../docs/heaps-and-design/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 12 ⭐ problems first.

**Recognise it when:** you need the *nearest* greater/smaller element, matching brackets, nested structure (decode, calculators), or "undo the most recent thing".

1. **Monotonic stack invariant:** keep indices whose values are increasing (or decreasing). When a new value breaks it, every popped index has just found its answer.
2. **Contribution technique:** for each element, `left` = distance to previous smaller, `right` = distance to next smaller → it is the minimum of `left × right` subarrays. Use `<` on one side and `≤` on the other to avoid double counting duplicates.
3. **Expression parsing:** one stack for numbers, apply `*`/`/` immediately, push `±num`; recursion or a stack frame for parentheses.
4. **Greedy + stack** (Remove K Digits): pop while the top is larger and you still have deletions.
5. Interviewers at both companies like **calculator** and **"visible people / prices"** variants; know the one-pass versions.

---

## 1. Classic Stack

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 20 | [Valid Parentheses](https://leetcode.com/problems/valid-parentheses/) | Easy | push opens, match closes | `G` `U` 🔥 |
| ☐ | 155 | [Min Stack](https://leetcode.com/problems/min-stack/) ⭐ | Medium | store (value, current min) | `G` `U` 🔥 |
| ☐ | 150 | [Evaluate Reverse Polish Notation](https://leetcode.com/problems/evaluate-reverse-polish-notation/) | Medium | operand stack | `G` |
| ☐ | 71 | [Simplify Path](https://leetcode.com/problems/simplify-path/) | Medium | split by `/`, stack of names | `G` `U` |
| ☐ | 394 | [Decode String](https://leetcode.com/problems/decode-string/) ⭐ | Medium | stack of (string, count) | `G` |
| ☐ | 735 | [Asteroid Collision](https://leetcode.com/problems/asteroid-collision/) ⭐ | Medium | simulate collisions | `G` `U` |
| ☐ | 636 | [Exclusive Time of Functions](https://leetcode.com/problems/exclusive-time-of-functions/) ⭐ | Medium | stack of running functions | `G` `U` |
| ☐ | 1249 | [Minimum Remove to Make Valid Parentheses](https://leetcode.com/problems/minimum-remove-to-make-valid-parentheses/) | Medium | indices of unmatched brackets | `G` |
| ☐ | 921 | [Minimum Add to Make Parentheses Valid](https://leetcode.com/problems/minimum-add-to-make-parentheses-valid/) | Medium | balance counter | `G` |

## 2. Expression Evaluation

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 227 | [Basic Calculator II](https://leetcode.com/problems/basic-calculator-ii/) ⭐ | Medium | apply * / immediately, sum the stack | `G` `U` |
| ☐ | 224 | [Basic Calculator](https://leetcode.com/problems/basic-calculator/) ⭐ | Hard | sign stack for parentheses | `G` `U` |

## 3. Monotonic Stack: Next Greater / Smaller

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 496 | [Next Greater Element I](https://leetcode.com/problems/next-greater-element-i/) | Easy | warm-up + hash map | `G` `U` 🔥 |
| ☐ | 739 | [Daily Temperatures](https://leetcode.com/problems/daily-temperatures/) ⭐ | Medium | decreasing stack of indices | `G` 🔥 |
| ☐ | 503 | [Next Greater Element II](https://leetcode.com/problems/next-greater-element-ii/) ⭐ | Medium | loop twice for circular | `G` 🔥 |
| ☐ | 1475 | [Final Prices With a Special Discount in a Shop](https://leetcode.com/problems/final-prices-with-a-special-discount-in-a-shop/) | Easy | next smaller-or-equal | `G` `U` 🔥 |
| ☐ | 901 | [Online Stock Span](https://leetcode.com/problems/online-stock-span/) | Medium | stack of (price, span) | `G` 🔥 |
| ☐ | 853 | [Car Fleet](https://leetcode.com/problems/car-fleet/) | Medium | sort by position, stack of times | `G` |
| ☐ | 1944 | [Number of Visible People in a Queue](https://leetcode.com/problems/number-of-visible-people-in-a-queue/) ⭐ | Hard | count pops + one | `G` 🔥 |
| ☐ | 962 | [Maximum Width Ramp](https://leetcode.com/problems/maximum-width-ramp/) | Medium | decreasing stack + scan from right | `G` |
| ☐ | 456 | [132 Pattern](https://leetcode.com/problems/132-pattern/) | Medium | scan right, track popped as `2` | `G` |

## 4. Monotonic Stack: Areas & Contribution

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 84 | [Largest Rectangle in Histogram](https://leetcode.com/problems/largest-rectangle-in-histogram/) ⭐ | Hard | previous/next smaller → width | `G` `U` 🔥 |
| ☐ | 907 | [Sum of Subarray Minimums](https://leetcode.com/problems/sum-of-subarray-minimums/) ⭐ | Medium | contribution: left × right | `G` |
| ☐ | 2104 | [Sum of Subarray Ranges](https://leetcode.com/problems/sum-of-subarray-ranges/) | Medium | sum of max contributions − min contributions | `G` |
| ☐ | 1856 | [Maximum Subarray Min-Product](https://leetcode.com/problems/maximum-subarray-min-product/) | Medium | min contribution × prefix sum | `G` `U` |

## 5. Greedy with a Monotonic Stack

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 402 | [Remove K Digits](https://leetcode.com/problems/remove-k-digits/) ⭐ | Medium | pop bigger digits while k > 0 | `G` 🔥 |
| ☐ | 316 | [Remove Duplicate Letters](https://leetcode.com/problems/remove-duplicate-letters/) | Medium | last occurrence + in-stack set (same as 1081) | `G` |

## 6. Premium Uber Favourites

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 1966 | [Binary Searchable Numbers in an Unsorted Array](https://leetcode.com/problems/binary-searchable-numbers-in-an-unsorted-array/) 🔒 | Medium | prefix max + suffix min | `G` `U` 🔥 |
| ☐ | 2282 | [Number of People That Can Be Seen in a Grid](https://leetcode.com/problems/number-of-people-that-can-be-seen-in-a-grid/) 🔒 | Medium | monotonic stack per row and column | `U` |

---

**Interview-ready when:** you can write a next-greater loop from memory in under 3 minutes and explain why each index is pushed and popped at most once (O(n)).
