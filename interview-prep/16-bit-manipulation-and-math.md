# Bit Manipulation, Math & Randomization: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Number theory and algebra](../docs/number-theory/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 13 ⭐ problems first.

**Recognise it when:** "without extra space", "without `*` or `/`", XOR pairs, overflow traps, primes, or "uniformly at random".

1. **XOR facts:** `a ^ a = 0`, `a ^ 0 = a`; `x & (x − 1)` clears the lowest set bit; `x & −x` isolates it.
2. **Overflow:** Google loves Reverse Integer / atoi / Divide Two Integers. Check before multiplying by 10, and handle `INT_MIN / −1`.
3. **Fast power:** square-and-multiply in O(log n); handle negative exponents.
4. **Sieve of Eratosthenes** for "count primes up to n".
5. **Randomization:** Fisher–Yates shuffle; reservoir sampling for streams of unknown length; rejection sampling (Rand10 from Rand7). Uber asks Shuffle an Array and Random Pick with Weight (in [Arrays](01-arrays-and-hashing.md)).

---

## 1. Bit Manipulation

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 136 | [Single Number](https://leetcode.com/problems/single-number/) ⭐ | Easy | XOR everything | `G` 🔥 |
| ☐ | 137 | [Single Number II](https://leetcode.com/problems/single-number-ii/) ⭐ | Medium | count each bit mod 3 | `G` 🔥 |
| ☐ | 260 | [Single Number III](https://leetcode.com/problems/single-number-iii/) | Medium | split by lowest differing bit | `G` |
| ☐ | 191 | [Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) | Easy | x & (x − 1) | `G` |
| ☐ | 338 | [Counting Bits](https://leetcode.com/problems/counting-bits/) ⭐ | Easy | dp[i] = dp[i >> 1] + (i & 1) | `G` |
| ☐ | 190 | [Reverse Bits](https://leetcode.com/problems/reverse-bits/) | Easy | swap bit by bit | `G` |
| ☐ | 268 | [Missing Number](https://leetcode.com/problems/missing-number/) | Easy | XOR indices with values | `G` 🔥 |
| ☐ | 371 | [Sum of Two Integers](https://leetcode.com/problems/sum-of-two-integers/) ⭐ | Medium | carry = (a & b) << 1 | `G` 🔥 |
| ☐ | 201 | [Bitwise AND of Numbers Range](https://leetcode.com/problems/bitwise-and-of-numbers-range/) | Medium | common prefix of m and n | `G` |
| ☐ | 2571 | [Minimum Operations to Reduce an Integer to 0](https://leetcode.com/problems/minimum-operations-to-reduce-an-integer-to-0/) | Medium | handle runs of 1s greedily | `G` `U` 🔥 |

## 2. Integer Arithmetic & Overflow

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 7 | [Reverse Integer](https://leetcode.com/problems/reverse-integer/) ⭐ | Medium | check overflow before ×10 | `G` 🔥 |
| ☐ | 8 | [String to Integer (atoi)](https://leetcode.com/problems/string-to-integer-atoi/) ⭐ | Medium | parse, clamp, handle signs | `G` `U` 🔥 |
| ☐ | 29 | [Divide Two Integers](https://leetcode.com/problems/divide-two-integers/) ⭐ | Medium | doubling subtraction | `G` 🔥 |
| ☐ | 50 | [Pow(x, n)](https://leetcode.com/problems/powx-n/) ⭐ | Medium | fast exponentiation | `G` 🔥 |
| ☐ | 9 | [Palindrome Number](https://leetcode.com/problems/palindrome-number/) | Easy | reverse half the number | `G` 🔥 |
| ☐ | 13 | [Roman to Integer](https://leetcode.com/problems/roman-to-integer/) | Easy | subtract when smaller precedes larger | `G` `U` 🔥 |
| ☐ | 12 | [Integer to Roman](https://leetcode.com/problems/integer-to-roman/) | Medium | greedy over value table | `G` 🔥 |
| ☐ | 43 | [Multiply Strings](https://leetcode.com/problems/multiply-strings/) | Medium | grade-school multiplication | `G` |

## 3. Number Theory & Geometry

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 204 | [Count Primes](https://leetcode.com/problems/count-primes/) ⭐ | Medium | sieve of Eratosthenes | `G` `U` |
| ☐ | 172 | [Factorial Trailing Zeroes](https://leetcode.com/problems/factorial-trailing-zeroes/) | Medium | count factors of 5 | `G` |
| ☐ | 149 | [Max Points on a Line](https://leetcode.com/problems/max-points-on-a-line/) ⭐ | Hard | slope as reduced (dy, dx) | `G` |
| ☐ | 564 | [Find the Closest Palindrome](https://leetcode.com/problems/find-the-closest-palindrome/) ⭐ | Hard | mirror prefix ± 1 candidates | `U` 🔥 |
| ☐ | 780 | [Reaching Points](https://leetcode.com/problems/reaching-points/) | Hard | work backwards with modulo | `G` |
| ☐ | 1823 | [Find the Winner of the Circular Game](https://leetcode.com/problems/find-the-winner-of-the-circular-game/) | Medium | Josephus recurrence | `G` |

## 4. Randomization

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 384 | [Shuffle an Array](https://leetcode.com/problems/shuffle-an-array/) ⭐ | Medium | Fisher–Yates | `G` `U` 🔥 |
| ☐ | 382 | [Linked List Random Node](https://leetcode.com/problems/linked-list-random-node/) ⭐ | Medium | reservoir sampling | `G` |
| ☐ | 398 | [Random Pick Index](https://leetcode.com/problems/random-pick-index/) | Medium | reservoir sampling over matches | `G` |
| ☐ | 470 | [Implement Rand10() Using Rand7()](https://leetcode.com/problems/implement-rand10-using-rand7/) | Medium | rejection sampling | `G` |
| ☐ | 710 | [Random Pick with Blacklist](https://leetcode.com/problems/random-pick-with-blacklist/) | Hard | remap blacklisted low indices | `G` `U` |
| ☐ | 519 | [Random Flip Matrix](https://leetcode.com/problems/random-flip-matrix/) | Medium | virtual swap with hash map | `G` |
| ☐ | 497 | [Random Point in Non-overlapping Rectangles](https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/) | Medium | weighted rectangle pick + uniform point | `G` `U` |

---

**Interview-ready when:** you can spot an overflow bug in your own code before running it and explain why your random algorithm is uniform.
