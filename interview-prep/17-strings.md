# Strings: Parsing, Matching & Hashing: Google / Uber Level

[← All interview topics](README.md) · Handbook: [String algorithms](../docs/strings/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 9 ⭐ problems first.

**Recognise it when:** parsing/formatting text, pattern search, repeated substrings, or palindromes built from prefixes/suffixes. (Two-string DP such as Edit Distance lives in the [DP list](15-dynamic-programming.md); windows live in [Sliding Window](03-sliding-window.md).)

1. **Simulation questions** (Text Justification, Zigzag, Remove Comments) are graded on clean code: helper functions, clear indices, no special-case pile-ups.
2. **KMP prefix function**: `pi[i]` = length of the longest proper prefix of `s[0..i]` that is also a suffix. Builds in O(n); use it for "first occurrence", "shortest palindrome", "longest happy prefix".
3. **Rolling hash (Rabin–Karp):** O(1) substring comparison; combine with binary search on length for "longest duplicate substring". Mention collisions and double hashing.
4. **Subsequence checks:** for many queries, precompute next-occurrence positions or bucket words by the next needed character.
5. Know `std::string_view`, `substr` cost (O(len)), and avoid quadratic concatenation.

---

## 1. Parsing & Simulation

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 6 | [Zigzag Conversion](https://leetcode.com/problems/zigzag-conversion/) ⭐ | Medium | row index bouncing | `G` 🔥 |
| ☐ | 68 | [Text Justification](https://leetcode.com/problems/text-justification/) ⭐ | Hard | greedy line fill + space distribution | `G` `U` |
| ☐ | 722 | [Remove Comments](https://leetcode.com/problems/remove-comments/) | Medium | state machine for block comments | `G` `U` |
| ☐ | 271 | [Encode and Decode Strings](https://leetcode.com/problems/encode-and-decode-strings/) ⭐ 🔒 | Medium | length-prefix encoding | `G` |
| ☐ | 833 | [Find And Replace in String](https://leetcode.com/problems/find-and-replace-in-string/) | Medium | sort replacements by index | `G` |
| ☐ | 809 | [Expressive Words](https://leetcode.com/problems/expressive-words/) | Medium | run-length groups compare | `G` |
| ☐ | 151 | [Reverse Words in a String](https://leetcode.com/problems/reverse-words-in-a-string/) | Medium | reverse whole, then each word | `G` 🔥 |

## 2. Subsequences

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 792 | [Number of Matching Subsequences](https://leetcode.com/problems/number-of-matching-subsequences/) ⭐ | Medium | bucket words by next needed char | `G` `U` |
| ☐ | 1055 | [Shortest Way to Form String](https://leetcode.com/problems/shortest-way-to-form-string/) 🔒 | Medium | next-occurrence table | `G` |

## 3. Pattern Matching (KMP / Z)

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 28 | [Find the Index of the First Occurrence in a String](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/) ⭐ | Easy | implement KMP | `G` 🔥 |
| ☐ | 459 | [Repeated Substring Pattern](https://leetcode.com/problems/repeated-substring-pattern/) | Easy | check pi[n−1] and period | `G` |
| ☐ | 686 | [Repeated String Match](https://leetcode.com/problems/repeated-string-match/) | Medium | repeat until length ≥ |b|, then search | `G` `U` |
| ☐ | 1392 | [Longest Happy Prefix](https://leetcode.com/problems/longest-happy-prefix/) ⭐ | Hard | prefix function, answer pi[n−1] | `G` |
| ☐ | 214 | [Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome/) ⭐ | Hard | KMP on s + '#' + reverse(s) | `G` `U` |
| ☐ | 796 | [Rotate String](https://leetcode.com/problems/rotate-string/) | Easy | search goal in s + s | `G` 🔥 |

## 4. Rolling Hash

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 187 | [Repeated DNA Sequences](https://leetcode.com/problems/repeated-dna-sequences/) ⭐ | Medium | hash fixed-length windows | `G` |
| ☐ | 1044 | [Longest Duplicate Substring](https://leetcode.com/problems/longest-duplicate-substring/) ⭐ | Hard | binary search length + rolling hash | `G` |
| ☐ | 1316 | [Distinct Echo Substrings](https://leetcode.com/problems/distinct-echo-substrings/) | Hard | rolling hash compare halves | `G` |
| ☐ | 3023 | [Find Pattern in Infinite Stream I](https://leetcode.com/problems/find-pattern-in-infinite-stream-i/) 🔒 | Medium | rolling hash over a stream | `U` |

## 5. Interactive

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 843 | [Guess the Word](https://leetcode.com/problems/guess-the-word/) | Hard | minimax guess choice | `G` |

---

**Interview-ready when:** you can write the KMP prefix function from memory and produce clean, bug-free simulation code for text-formatting questions on the first try.
