# Tries (Prefix Trees): Google / Uber Level

[← All interview topics](README.md) · Handbook: [String algorithms §3 (trie)](../docs/strings/README.md), [Heaps and data-structure design §4 (XOR trie)](../docs/heaps-and-design/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 7 ⭐ problems first.

**Recognise it when:** many words share prefixes, you need autocomplete or prefix counts, you search a grid for *many* words, or you maximise XOR.

1. Node = `children[26]` (or a map) + `isEnd` / `count` / stored word. Store whatever the query needs **at the node**.
2. **Word Search II:** put the words in a trie and DFS the grid once; prune finished leaves to avoid TLE.
3. **Binary trie** for XOR: insert numbers bit by bit from the top; greedily choose the opposite bit.
4. **Design questions** (file system, autocomplete) are tries with extra data per node — Uber asks these often.
5. Always compare with the hash-set alternative and explain the memory trade-off.

---

## 1. Core

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 208 | [Implement Trie (Prefix Tree)](https://leetcode.com/problems/implement-trie-prefix-tree/) ⭐ | Medium | insert / search / startsWith | `G` `U` |
| ☐ | 211 | [Design Add and Search Words Data Structure](https://leetcode.com/problems/design-add-and-search-words-data-structure/) ⭐ | Medium | DFS on `.` wildcard | `G` |
| ☐ | 14 | [Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix/) | Easy | warm-up: vertical scan or trie | `G` `U` 🔥 |
| ☐ | 648 | [Replace Words](https://leetcode.com/problems/replace-words/) | Medium | shortest root on the path | `G` `U` |
| ☐ | 1268 | [Search Suggestions System](https://leetcode.com/problems/search-suggestions-system/) ⭐ | Medium | trie or sort + binary search | `G` `U` |
| ☐ | 3043 | [Find the Length of the Longest Common Prefix](https://leetcode.com/problems/find-the-length-of-the-longest-common-prefix/) | Medium | trie of one array, walk the other | `G` `U` |
| ☐ | 1233 | [Remove Sub-Folders from the Filesystem](https://leetcode.com/problems/remove-sub-folders-from-the-filesystem/) | Medium | sort, or trie with end markers | `G` `U` |

## 2. Trie + DFS / Backtracking

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 212 | [Word Search II](https://leetcode.com/problems/word-search-ii/) ⭐ | Hard | trie + grid DFS + pruning | `G` `U` 🔥 |
| ☐ | 676 | [Implement Magic Dictionary](https://leetcode.com/problems/implement-magic-dictionary/) | Medium | DFS with one allowed mismatch | `G` |
| ☐ | 336 | [Palindrome Pairs](https://leetcode.com/problems/palindrome-pairs/) | Hard | trie of reversed words | `G` |
| ☐ | 1032 | [Stream of Characters](https://leetcode.com/problems/stream-of-characters/) | Hard | trie of reversed words + stream buffer | `G` |

## 3. Trie-based Design

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 642 | [Design Search Autocomplete System](https://leetcode.com/problems/design-search-autocomplete-system/) ⭐ 🔒 | Hard | node stores top-3 sentences | `G` `U` |
| ☐ | 588 | [Design In-Memory File System](https://leetcode.com/problems/design-in-memory-file-system/) ⭐ 🔒 | Hard | trie of path segments | `G` `U` 🔥 |
| ☐ | 1166 | [Design File System](https://leetcode.com/problems/design-file-system/) 🔒 | Medium | trie or hash map of paths | `G` `U` |

## 4. Lexicographic & Counting

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 386 | [Lexicographical Numbers](https://leetcode.com/problems/lexicographical-numbers/) | Medium | pre-order on a virtual 10-ary trie | `G` |
| ☐ | 440 | [K-th Smallest in Lexicographical Order](https://leetcode.com/problems/k-th-smallest-in-lexicographical-order/) | Hard | count subtree sizes, skip ranges | `G` |
| ☐ | 2416 | [Sum of Prefix Scores of Strings](https://leetcode.com/problems/sum-of-prefix-scores-of-strings/) | Hard | prefix counts on nodes | `G` |

## 5. Binary (XOR) Trie

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 421 | [Maximum XOR of Two Numbers in an Array](https://leetcode.com/problems/maximum-xor-of-two-numbers-in-an-array/) ⭐ | Medium | greedy opposite bit | `G` |
| ☐ | 1707 | [Maximum XOR With an Element From Array](https://leetcode.com/problems/maximum-xor-with-an-element-from-array/) | Hard | offline: sort queries, insert progressively | `G` |

---

**Interview-ready when:** you can implement a trie in under 5 minutes and explain why Word Search II with a trie beats running Word Search per word.
