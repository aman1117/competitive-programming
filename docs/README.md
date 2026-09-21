# Algorithm handbook

[Project home](../README.md)

**Learn the reasoning behind the templates, not just the syntax.** This handbook is written for a C++ programmer developing strong LeetCode contest skills and the core Codeforces Expert repertoire.

The chapters are explanation-oriented: each algorithm or closely related family has a use case, an invariant or recurrence, a worked example, implementation usage, and a complexity explanation. Supporting foundational patterns are included so the harder topics do not depend on unexplained prerequisites.

## Chapters

| Chapter | Main questions it answers | Implementation |
|---|---|---|
| [Merge sort and quicksort](sorting/README.md) | How do stable merging and randomized partitioning differ? | `sorting.hpp` |
| [Search, prefixes, and windows](search-and-windows/README.md) | When can a boundary move only forward? What makes binary search valid? | `arrays.hpp`, `patterns.hpp` |
| [Monotonic structures and greedy](monotonic-and-greedy/README.md) | Why can candidates be discarded? How do elements own subarrays? | `arrays.hpp` |
| [Range queries and offline processing](range-queries/README.md) | Fenwick or segment tree? Why does lazy propagation work? | `data_structures.hpp`, `offline.hpp` |
| [Graph traversal and shortest paths](graph-paths/README.md) | BFS, 0-1 BFS, or Dijkstra? What belongs in a visited state? | `graphs.hpp`, `patterns.hpp` |
| [Connectivity, MST, and 2-SAT](connectivity/README.md) | How do components, low links, and implications solve global constraints? | `connectivity.hpp`, `graphs.hpp` |
| [Flow and matching](flow-and-matching/README.md) | Why are reverse edges necessary? What is an augmenting path? | `flow.hpp` |
| [Trees, ancestors, and rerooting](trees/README.md) | What state returns to a parent? How can every root be processed efficiently? | `trees.hpp`, `nodes.hpp`, `offline.hpp` |
| [String algorithms](strings/README.md) | How do failure links, Z-boxes, hashes, and palindrome mirrors reuse work? | `strings.hpp` |
| [Sequence and state-machine DP](sequence-dp/README.md) | What does a DP cell mean? Why does update order change the problem? | `dp.hpp` |
| [Interval, bitmask, and digit DP](advanced-dp/README.md) | How should subsets, intervals, and huge numeric bounds become states? | `dp.hpp` |
| [Backtracking and meet in the middle](backtracking/README.md) | What can be pruned? What must be undone? How expensive is enumeration? | `search.hpp` |
| [Number theory and algebra](number-theory/README.md) | When does modular division exist? How do CRT and matrix powers work? | `math.hpp` |
| [Heaps and data-structure design](heaps-and-design/README.md) | How do invariants support medians, caching, XOR, and random selection? | `data_structures.hpp`, `arrays.hpp` |
| [Linked-list algorithms](linked-lists/README.md) | How can links be changed without losing nodes or allocating new ones? | `nodes.hpp` |
| [Integer geometry](geometry/README.md) | How do orientation tests support intersections and convex hulls? | `geometry.hpp` |

## How to run the examples

For the on-platform LC revision tracker, local sample/stress runners, debugger, and submission exporter, use the separate **[Practice workflow guide](practice-workflow/README.md)**. It is a how-to guide, not another algorithm chapter.

Every `cpp` block in a topic chapter is an independent **`main()` body snippet**, not a complete judge submission. Put one snippet inside this wrapper and save it as `practice\example.cpp`:

```cpp
#include "cp/all.hpp"
#include <iostream>

int main() {
    // Replace this demonstration with one chapter snippet.
    auto answer = cp::min_jumps({2, 3, 1, 1, 4});
    assert(answer == 2);
}
```

From the project directory:

```powershell
cprun .\practice\example.cpp
```

Assertions describe the expected result; a successful example may intentionally print nothing. Do not compile with `NDEBUG` if you want those assertions to run. For submission, copy only the required implementation and dependencies: judges cannot access your local `cp/...` headers. On LeetCode, put the logic behind the required `Solution` method and omit your local driver.

## Reading conventions

| Term | Convention |
|---|---|
| Indices and vertices | Zero-based |
| Array ranges | Half-open `[l,r)`, unless explicitly stated otherwise |
| TC | Worst-case time unless marked expected or amortized |
| SC | Auxiliary space including recursion and input copies; excludes returned output unless stated |
| Data-structure space | Total retained storage, including preprocessing |
| Empty inputs | Follow each API's contract; not every algorithm accepts them |
| `n`, `m` | Sequence lengths, unless a section defines otherwise |
| `V`, `E` | Vertex and edge counts |
| `h`, `w` | Tree height and maximum width; neither is automatically logarithmic |
| `INF` | A sentinel, not arithmetic infinity; finite DP/path computations must stay within the documented bounds |

Asymptotic statements suppress fixed empty-input overhead. A logarithmic bound normally means `log(n+1)` near zero. Fixed alphabets such as 26 letters or 256 bytes are stated explicitly when useful.

### Three habits that prevent wrong complexity claims

1. Count **total pointer movements**, not how many loops appear in the source. Two nested loops can be linear if each pointer advances at most `n` times.
2. Count the actual state space and transitions. `n*2^n` states with `n` transitions per state cost `n^2*2^n`, not merely `2^n`.
3. Count memory that really exists: recursion, sorted copies, heap duplicates, trie nodes, and retained container capacity. Output size matters when enumerating solutions.

## A productive reading sequence

Read search/windows and monotonic/greedy first. Then study range structures, graph paths, sequence DP, and trees. Follow with string algorithms, backtracking, and advanced DP. Use connectivity, number theory, flow, and geometry as problem-driven extensions. Heaps/design and linked lists support broader LeetCode mastery.

For each section, trace the example by hand, identify the invariant, change one assumption, and explain why the method still works or fails. The [main LC pattern map](../README.md#leetcode-first-pattern-map) supplies representative practice problems.

## Scope and source of truth

This handbook documents the algorithms implemented in this repository, including supporting patterns and public helpers. Related operations are explained together rather than creating a nearly empty page for every method. Standard-library APIs are discussed where they support a pattern; this is not a replacement for a complete C++ reference.

The linked headers are authoritative for signatures, preconditions, and sentinel behavior. Advanced topics absent from the implementation, such as persistent trees and FFT/NTT, are not presented as existing features. Neither this handbook nor the template collection guarantees a contest rating.
