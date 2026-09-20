# String algorithms

[Handbook](../README.md) | [Implementation](../../include/cp/strings.hpp)

**Prerequisites:** arrays, prefixes/suffixes, and amortized analysis.  
**Goal:** reuse already-known matching information rather than restart comparisons at every position.

## 1. Prefix function and KMP

**Use when:** exact pattern matching, borders, or repeated prefix/suffix structure matters.

`pi[i]` is the length of the longest proper prefix of `s[0..i]` that is also a suffix. If extending a candidate border fails, its own longest border is the next possible candidate: jump to `pi[j-1]` instead of decrementing `j` one character at a time.

For `"ababcabab"`, the prefix lengths are `[0,0,1,2,0,1,2,3,4]`. At the `c`, the candidate `"ab"` fails, and no nonempty border remains. Later, the final `"abab"` matches the beginning.

```cpp
assert((cp::prefix_function("ababcabab") == std::vector<int>{0, 0, 1, 2, 0, 1, 2, 3, 4}));
assert((cp::kmp_search("ababa", "aba") == std::vector<int>{0, 2}));
```

KMP maintains how much of the pattern has matched while scanning the text. After matching `"aba"` at position zero, fall back to border length one instead of clearing the state: the trailing `"a"` may start the overlapping match at position two.

**TC/SC:** prefix computation is `O(n)` time with `O(n)` output and `O(1)` auxiliary space. KMP is `O(text_length+pattern_length)` time and `O(pattern_length)` workspace, excluding matches. The matched length increases only linearly many times, so all fallback decreases together are linear too.

An empty pattern matches every boundary, including the boundary after the last character. This API reports overlapping matches; a naive reset after each match would miss some.

## 2. Z-function

**Use when:** the longest common prefix of the whole string with every suffix is needed.

`z[i]` is the length of the prefix matching the suffix beginning at `i`; this implementation defines `z[0]=0`.

Maintain a rightmost matching box `[l,r)`. If `i` lies inside it, previously calculated values tell you a safe initial match length. Only comparisons extending past the current right boundary need new work.

For `"aaaaa"`, index one matches four characters. Subsequent indices reuse that box and produce lengths three, two, and one.

```cpp
assert((cp::z_function("aaaaa") == std::vector<int>{0, 4, 3, 2, 1}));
```

**TC/SC:** `O(n)` time, `O(1)` auxiliary space excluding `O(n)` output. The right boundary advances at most `n` times. If combining `pattern + separator + text` yourself, choose a separator absent from both strings or use an encoding that cannot collide with input symbols.

## 3. Trie

**Use when:** operations depend on shared prefixes rather than arbitrary substrings.

A node represents one prefix, and a character selects the next edge. Word endings are marked separately from children: `"app"` can be a complete word and also a prefix of `"apple"`.

```cpp
cp::Trie trie;
trie.insert("apple");
int app = trie.insert("app");
trie.insert("app");
assert(trie.count("app") == 2);
assert(trie.prefix_count("ap") == 3);
assert(trie.count("ap") == 0);
assert(trie.terminal_count(app) == 2);
assert(trie.transition(0, 'z') == -1);
```

`through` counts inserted words passing through a node, including duplicates; `terminal` counts exact endings. `insert` returns the terminal node ID, which can carry a separate payload. Root ID is zero.

**TC/SC:** length-`L` insertion is amortized `O(L)` because growing the node vector occasionally reallocates; lookups are worst-case `O(L)`. Worst-case retained storage is `O(26*S)`, where `S` is total inserted characters plus the root. The alphabet is strictly lowercase `a-z`.

The same traversal primitives power [word-break DP](../sequence-dp/README.md) and [multiword grid search](../backtracking/README.md). A missing transition returns `-1`; never pass that sentinel back as a valid node.

## 4. Polynomial rolling hash

**Use when:** many substring comparisons need fast fingerprints and collision risk is acceptable.

Represent a prefix as a polynomial in a fixed base, reduced modulo a prime. A substring hash removes the earlier prefix multiplied by the appropriate base power:

```text
hash(l,r) = prefix[r] - prefix[l] * base^(r-l)  (mod modulus)
```

This normalization makes equal strings at different positions comparable without modular division.

```cpp
cp::RollingHash hash("abracadabra");
assert(hash.hash(0, 4) == hash.hash(7, 11)); // "abra" and "abra".
```

**TC/SC:** `O(n)` preprocessing time/storage and `O(1)` query time/workspace. Compare lengths as well as hashes.

**Correctness boundary:** equal hashes are not proof of equal strings, even with two moduli. Fixed bases can be attacked with adversarial inputs. Prefer KMP/Z for deterministic exact matching; verifying equal-hash candidates adds comparison work that must be included in the time bound.

## 5. Manacher's algorithm

**Use when:** all palindrome centers, the longest palindrome, or the total number of palindromic substrings are needed in linear time.

Maintain the rightmost known palindrome. A center inside it can mirror information from the symmetric center. Its initial radius is limited by the current palindrome boundary; explicit comparisons only extend beyond the information already known.

The implementation keeps separate arrays:

| Array | Meaning | Palindrome length |
|---|---|---|
| `odd[i]` | Radius including center `i` | `2*odd[i]-1` |
| `even[i]` | Radius around the gap between `i-1` and `i` | `2*even[i]` |

For `"ababa"`, odd radii are `[1,2,3,2,1]`. The center radius three represents lengths one, three, and five. Summing every radius counts every nonempty palindrome once.

```cpp
auto odd = cp::manacher("ababa");
assert((odd.odd == std::vector<int>{1, 2, 3, 2, 1}));
auto total = std::accumulate(odd.odd.begin(), odd.odd.end(), 0LL)
           + std::accumulate(odd.even.begin(), odd.even.end(), 0LL);
assert(total == 9);
assert(cp::manacher("abba").even[2] == 2);
```

**TC/SC:** `O(n)` time, `O(1)` auxiliary space excluding `O(n)` output arrays. Successful new comparisons advance the right boundary only linearly many times. Be precise about whether an even center is before or after index `i`; off-by-one errors usually come from mixing conventions.

## 6. Palindrome table

**Use when:** a DP or backtracking routine needs arbitrary palindrome interval queries and quadratic memory is acceptable.

For inclusive endpoints:

```text
pal[l][r] = (s[l]==s[r]) AND (r-l<2 OR pal[l+1][r-1])
```

Process left endpoints from right to left so the inner interval is already known.

```cpp
auto pal = cp::palindrome_table("aab");
assert(pal[0][1]);  // "aa".
assert(!pal[0][2]); // "aab".
```

**TC/SC:** `O(n^2)` time, `O(n^2)` returned table, `O(1)` additional workspace. Unlike most range APIs here, table indices are **inclusive**. Use it with [palindrome minimum cuts](../advanced-dp/README.md) and [partition enumeration](../backtracking/README.md). Manacher has better preprocessing bounds, but this table is often easier to integrate into interval DP.
