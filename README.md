# Competitive programming toolkit

A **LeetCode-first C++17 toolkit**, also covering the core Codeforces Expert repertoire. It includes reusable patterns from fundamentals through intermediate/contest-hard work, with several stretch topics. Start with the [LeetCode pattern map](#leetcode-first-pattern-map), then consult the [algorithm index](#algorithm-index) for exact implementations and complexity.

**No honest toolkit is "100% enough" for every Guardian/Expert problem or guarantees either rank.** This library covers a broad, concrete core syllabus; ratings also depend on recognizing combinations of patterns, proving correctness, implementing adaptations, and performance under contest time limits. Specialized harder topics outside this scope are explicitly listed below, rather than silently labeled covered.

## Algorithm explanations and worked examples

Read the **[Algorithm handbook](docs/README.md)** for 15 topic-specific READMEs covering the implemented algorithms and their prerequisites. Each chapter explains when to use its techniques, the invariant or recurrence, worked examples, C++ usage, time/space reasoning, and common failure cases.

Start with [search and windows](docs/search-and-windows/README.md), [monotonic and greedy patterns](docs/monotonic-and-greedy/README.md), and [sequence DP](docs/sequence-dp/README.md). For deeper material, see [range queries](docs/range-queries/README.md), [graph paths](docs/graph-paths/README.md), [tree algorithms](docs/trees/README.md), and [interval/bitmask/digit DP](docs/advanced-dp/README.md). The handbook index links every chapter and its source headers.

## Start here

Your workspace is `C:\Users\amansha\competitive-programming`.

```powershell
cd "$HOME\competitive-programming"

# Open a NEW PowerShell 7 terminal, or load the new shortcut in this terminal:
. $PROFILE.CurrentUserAllHosts

cprun                                  # Compile .\main.cpp, run with keyboard input
cprun .\main.cpp -InputFile .\input.txt  # Run with input from a file
cprun .\practice\a.cpp -Local           # Debug symbols, -O0, LOCAL debug output
cprun .\templates\leetcode.cpp -Local  # LeetCode starter's local driver (prints 6)

# A working range-sum example (expected output: 15, 16, 10 on separate lines):
cprun .\examples\range_sum.cpp -InputFile .\examples\range_sum.in

# Copy the starter for a new problem:
Copy-Item .\main.cpp .\practice\a.cpp

# Run the library's self-contained tests (no external test framework):
.\scripts\test.ps1
```

`cprun` is installed in your **current-user/all-hosts PowerShell 7 profile**, so it works from any working directory in new PowerShell 7 sessions. It is not a Bash or cmd.exe alias. Source/input paths are relative to your current directory; quote paths containing spaces. The default compiler is the already-installed `g++`; use `-Compiler clang++` to select another GCC/Clang-compatible driver.

The shortcut stops on compiler errors, reports nonzero program exit codes, and removes its uniquely named executable afterward. Builds use C++17, `-Wall -Wextra -Wshadow`, and `-O2` by default. `-Local` adds `-DLOCAL -g -O0`; it does **not** enable a sanitizer. Input-file redirection preserves file bytes. Without `-InputFile`, finish manual input as your console requires (usually Ctrl+Z then Enter on Windows), or let the program finish after reading the expected input.

Without the profile shortcut, call the script directly:

```powershell
& "$HOME\competitive-programming\scripts\run-cpp.ps1" .\solution.cpp -InputFile .\input.txt
```

## C++ hover explanations in VS Code

The **clangd** extension (`llvm-vs-code-extensions.vscode-clangd`) is installed in VS Code Insiders and recommended by this workspace. It uses the existing LLVM-MinGW language server/compiler; no second C++ language server is needed.

Open **this project folder**, not just an isolated `.cpp` file, so its compiler/include settings are loaded:

```powershell
code "$HOME\competitive-programming"
```

Open `examples\hover_demo.cpp` and hover over `Fenwick`, `add`, `sum`, `count_inversions`, or `unite`. The popups show signatures and the declaration documentation: purpose, assumptions, return behavior, and TC/SC. On first open, allow a few seconds for header indexing; an early hover may initially show only the signature. You can also request a hover with **Ctrl+K, then Ctrl+I**. If the folder was already open during installation, run **Developer: Reload Window** from the command palette.

Library declaration comments use Doxygen-compatible `///` syntax. For your own functions, put documentation immediately **above the declaration**, not only inside its body:

```cpp
/// @brief Return the square of x.
/// @param x Input whose square fits in long long.
/// @return x multiplied by itself.
/// @note TC O(1); SC O(1).
long long square(long long x) {
    return x * x;
}
```

This is documentation-aware hover help, not automatic AI explanation of arbitrary undocumented code. An undocumented function may show only its signature/type information; standard-library descriptions depend on the installed headers.

Configuration is kept in `.vscode\settings.json`, `.clangd`, and `compile_flags.txt`. The flags specify C++17 and the local `include` directory. Compiler/server paths are pinned to the LLVM installation on this machine; after moving the project or changing the toolchain, update `clangd.path`, the trusted `--query-driver` path, and `.clangd`'s `CompileFlags.Compiler`.

clangd activates only in trusted workspaces. Trust this folder only if you trust its contents. If hover help is missing, check the **clangd** output channel or run **clangd: Restart language server**. Avoid running another C++ IntelliSense provider alongside clangd in this workspace, which can produce duplicate/conflicting results.

## Workspace

```text
competitive-programming\
  main.cpp                 Standalone Codeforces submission starter
  input.txt                Scratch input (starts with t=1)
  templates\leetcode.cpp   LeetCode class/method starter + LOCAL-only sample driver
  include\cp\              Reusable, commented algorithm headers
  docs\README.md           Algorithm handbook and chapter index
  docs\<topic>\README.md   Worked explanations, examples, invariants, and TC/SC
  examples\                Runnable Fenwick example and sample input/output
  examples\hover_demo.cpp  Small program for trying documented function hovers
  practice\                Your solutions
  tests\                   Deterministic and seeded differential tests
  scripts\run-cpp.ps1       Compile-and-run implementation for cprun
  scripts\test.ps1          Header, starter, and algorithm checks
  .vscode\                 clangd extension recommendation and editor settings
  .clangd                  Compiler selection for the language server
  compile_flags.txt        C++17/include flags shared by editor analysis
  .build\                  Ignored temporary executables
```

### Codeforces and LeetCode use

`main.cpp` is standalone and uses standard C++17 headers, not compiler-specific `bits/stdc++.h`, PBDS, or `#define int long long`. Its `solve()` is intentionally empty. It reads a leading test-case count by default: set `MULTIPLE_TEST_CASES = false` for problems without `t`. Reset all per-test state inside `solve()`. `debug(...)` prints streamable values to stderr only with `-Local`.

For local practice you may include a specific header, or the whole library:

```cpp
#include "cp/all.hpp"  // cprun adds the toolkit's include directory

void solve() {
    int n;
    std::cin >> n;
    std::vector<cp::i64> a(n);
    for (auto& x : a) std::cin >> x;
    cp::Fenwick bit(a);
    std::cout << bit.sum(0, n) << '\n';
}
```

**Do not submit local `cp/...` includes to a judge.** Copy the needed function/class and its dependencies into the standalone `main.cpp` before submission. All implementation code is original and header-based. Headers use namespace `cp`; keep the namespace (and call `cp::...`) or remove it consistently. `common.hpp` contains standard includes, `i64`, `INF`, and `MOD`. For a single function, copy only the standard includes/types/constants it actually needs. Local dependencies are explicit at the top of each header; `graphs.hpp` also uses `DSU`, and tree/connectivity/flow headers depend on graph types/helpers.

For LeetCode, replace the example method in `templates\leetcode.cpp` with the exact requested signature. Do not submit `main()`, and do not redefine judge-provided node types. `nodes.hpp` supplies compatible local practice types; paste only the relevant helper when the platform already provides `ListNode` or `TreeNode`. When copying `binary_tree_stats` or `binary_tree_lca`, also copy their `binary_tree_fold` dependency.

Use `cprun .\templates\leetcode.cpp -Local` to run its guarded local driver; update that driver's sample calls when changing the method signature. Without `-Local`, this file intentionally has no `main()` because LeetCode provides it. The guarded driver is excluded from the judge build.

## LeetCode-first pattern map

This is a **recognition and practice map**, not a claim that every listed problem can use a template unchanged. Library functions intentionally expose explicit conventions (ranges, sentinels, modulo counts, mutation). Adapt them to the judge's exact signature and requirements.

| Pattern family | Available implementation / technique | Representative LC practice |
|---|---|---|
| Hash lookup, grouping, membership | Standard `map`/`unordered_map`/`set` recipes; check complement before inserting; frequency-vector keys for anagrams | 1 Two Sum, 49 Group Anagrams, 128 Longest Consecutive Sequence |
| Prefix/suffix, prefix frequencies | `prefix_sums`, `Prefix2D`, `product_except_self`, `count_subarrays_sum`, `count_subarrays_xor` | 238, 304, 560; for 974 count equal normalized remainders |
| Sorted two pointers and duplicate handling | `two_sum_sorted`, `three_sum`, `trapped_water` | 167, 15, 42 |
| In-place placement / three-way partition / voting | `first_missing_positive`, `sort_three_values`, `majority_element` | 41, 75, 169 |
| Variable windows | `minimum_window`, `longest_unique_substring`, `longest_ones_after_flips`, `count_product_less`, `subarrays_exactly_k_distinct` | 76, 3, 1004, 713, 992 |
| Monotone stack/deque and contributions | `next_greater`, `sliding_max`, `largest_rectangle`, `maximal_rectangle`, `sum_subarray_minimums`, `shortest_subarray_at_least` | 739, 239, 84, 85, 907, 862 |
| Binary search and partition | STL bounds, `search_rotated`, `kth_two_sorted`, `first_true`, `split_min_largest_sum` | 34, 33/81, 4, 875, 410 |
| Greedy frontier / interval / sweep line | `min_jumps`, `merge_intervals`, `max_nonoverlapping`, `meeting_rooms` | 45/55, 56, 435, 253 |
| Heap, top-k, k-way merge, order statistics | STL heaps/`nth_element`, `merge_k_sorted`, `MedianFinder`, `sliding_median` | 215, 347, 23 (adapt to list nodes), 295, 480 |
| Linked lists / slow-fast / rewiring | `middle_node`, `cycle_entry`, `reverse_list`, `reverse_k_group`, `palindrome_list`, `list_intersection`, `sort_list` | 876, 142, 206, 25, 234, 160, 148 |
| Tree DFS/BFS/postorder DP | `inorder`, `level_order`, `is_bst`, `binary_tree_fold`, `binary_tree_stats` | 94, 102, 98, 104/110/543/124 |
| Tree paths and ancestors | `binary_tree_lca`, `count_tree_path_sum`, `BinaryLifting`, `tree_distance_sums` | 236, 437, 1483, 834 |
| Grid / multisource BFS / DAG | `grid_bfs`, `bfs`, `dfs_order`, `longest_increasing_path` | 200 (component traversal), 994/542 (adapt source/blocked rules), 329 |
| Topological dependencies / DSU | `topological_sort`, `DSU`, `bipartite_coloring` | 207/210, 684/721, 785 |
| Weighted paths / MST / connectivity | `dijkstra`, `zero_one_bfs`, `kruskal`, `bridges_and_articulations`, SCC | 743, 1368, 1584, 1192 |
| BFS with additional state | `shortest_visit_all`; general state must include mask/resources/last edge color | 847; adapt state for 864, 1293, 1129 |
| Subsets / duplicates / combinations / constraint search | `enumerate_subsets`, `enumerate_permutations`, `combination_sum`, `enumerate_parentheses`, `enumerate_n_queens` | 78, 47, 39/40, 22, 51/52 |
| Word/grid/trie backtracking | `word_exists`, `find_words`, `Trie` traversal primitives | 79, 212, 208 |
| 1D take/skip, product, stock state machines | `nonadjacent_sum`, `max_product_subarray`, `stock_profit_k`, `stock_profit_cooldown` | 198/213, 152, 121/122/123/188/714, 309 |
| Knapsack / subset / coin DP | `knapsack_01`, `knapsack_unbounded`, `subset_sum`, `min_coins`, `coin_combinations` | 416, 494 (derive sum transformation), 322, 518 |
| Sequence / string / segmentation DP | `lis_indices`, `lcs_length`, `edit_distance`, `distinct_subsequences`, `word_break`, `wildcard_match` | 300, 1143, 72, 115 (see modulo warning), 139, 44 |
| Interval / palindrome / game DP | `matrix_chain`, `burst_balloons`, `palindrome_min_cuts`, `enumerate_palindrome_partitions`, `take_ends_score_difference` | 312, 132, 131, 486 |
| Weighted scheduling / bitmask / digit DP | `weighted_scheduling`, `traveling_salesman`, `subset_zeta`, `count_digit_sum`, `count_unique_digit_numbers` | 1235, 698 (adapt subset state), 233 (adapt digit state), 2376 |
| Exact string matching / palindromes | KMP, Z, `manacher`, `palindrome_table`; double hashes only when collision risk is acceptable | 28, 5, 647, 214 (adapt prefix construction) |
| Object/data-structure design | `MinStack`, `LRUCache`, `RandomizedSet`, `Trie`, median structures | 155, 146, 380, 208, 295 |
| Harder range/offline work | Fenwick, segment/lazy trees, sparse table, Mo, coordinate compression | 307, 315 (right-to-left counts), 327 (prefix sums + compressed queries) |

For **Guardian preparation**, prioritize windows/contributions, greedy reasoning, binary-search feasibility, graph/state modeling, and DP state design before spending excessive time memorizing flow or exotic structures. LRU and linked-list rewiring are valuable general LeetCode skills but less central to contest rating than those families.

### How to derive variants instead of memorizing solutions

| Signal in a problem | Derivation to practice |
|---|---|
| "Exactly k" | Count `atMost(k)-atMost(k-1)` only when the counted sets satisfy that identity |
| Negative values break a window | Use prefix frequencies, ordered prefix data, or a monotone prefix deque |
| Sum over every subarray | Assign each element an ownership interval; make one duplicate boundary strict and the other non-strict |
| Minimum feasible capacity/time | Prove monotonicity, implement a feasibility check, multiply its cost by binary-search iterations |
| Two sorted arrays, kth/median | Partition by **element count**, not by averaging array medians |
| A node can be revisited with a different resource state | Visited key must include that state, not just the vertex |
| Removing something changes neighbors | Try choosing the **last** removal in an interval (balloons) |
| Tree result combines children | Return a subtree state; distinguish an upward-extendable path from a complete path |
| Huge numeric bound but few digits | Digit position + tight + started/mask/remainder; count the state dimensions |
| Around 40 subset choices | Meet in the middle, not a full 2^40 enumeration |
| Around 15-20 subset choices | Bitmask DP/BFS; estimate `n*2^n` memory before allocation |
| Dictionary search over many board paths | Carry a trie node along the path; do not run a separate full search per word |

Important adaptation traps: `distinct_subsequences` and `coin_combinations` are **modular** counters, whereas some LeetCode questions require exact counts. `sum_subarray_minimums` is also modular; an exact sum-of-ranges problem needs exact accumulation and symmetric maximum contributions. `shortest_visit_all` permits revisiting vertices; Held-Karp TSP does not. `wildcard_match` is not the `.`/`*` regular-expression matcher. `grid_bfs` uses `#` as the blocked-cell rule; not every grid question does.

## Contracts and complexity conventions

Read each function's comment before copying it. It states the required input, return/sentinel behavior, **TC**, **SC**, and the reason behind non-obvious bounds.

| Convention | Meaning |
|---|---|
| Indexing | Zero-based vertices and array indices |
| Ranges | Half-open `[l,r)`, unless an interval/geometry comment explicitly says otherwise |
| TC | Worst-case time unless explicitly labeled amortized/expected |
| SC | Auxiliary memory, including recursion and by-value input copies; excludes input and returned output unless specified |
| Stateful structures | Listed space is total retained storage; operation workspace is documented in code |
| Bounds | Container sizes and indices must fit `int`; dimensions must be nonnegative and realistically allocatable |
| Arithmetic | Sums, DP transitions, and path costs must stay strictly within `(-cp::INF, cp::INF)`; `INF = LLONG_MAX/4` is an unreachable sentinel, not infinity arithmetic |
| Preconditions | `assert` catches many misuse cases while enabled; these are contest templates, not fully validating public APIs |
| Empty input | Most routines support it; trees need a nonempty connected tree, sparse queries need `l<r`, and sliding maxima need `1<=k<=n` |
| Recursion | Graph DFS/SCC/low-link/tree preprocessing and binary-tree folds are iterative; Dinic/Kuhn and backtracking still recurse (depth documented in code) |
| Hashing | Double rolling hashes can collide; fixed bases are not adversarially secure |

Bounds below use positive dimensions and suppress constant empty-input overhead. `n,m` are sequence lengths, `V,E` graph vertices/edges, `q` queries, `W` capacity, `A` amount, `D` digits, `S` target digit sum, `B` bit width, `h` tree height, and `w` maximum tree width.

## Algorithm index

### Arrays, searching, sliding windows, greedy -- `arrays.hpp`, `patterns.hpp`

| Template | TC | SC / stored space | Use / important restriction |
|---|---|---|---|
| `prefix_sums` | Build O(n), query O(1) | O(1) auxiliary, O(n) result | Sum `[l,r)` via `p[r]-p[l]` |
| `Prefix2D` | Build O((R+1)(C+1)), query O(1) | O((R+1)(C+1)) stored | Rectangle inclusion-exclusion, padded boundaries |
| `range_additions` | O(n+q) | O(n) | Offline updates to an initially zero array |
| `compress` | O(n log n) | O(n) | Coordinate compression preserves order, not distances |
| `first_true` | O(log(range) * predicate cost) | O(1) + predicate workspace | Binary search on a monotone answer predicate; high bound exclusive |
| `two_sum_sorted` | O(n) | O(1) | Two pointers; input already sorted |
| `min_length_nonnegative` | O(n) | O(1) | Nonnegative values, positive target |
| `shortest_subarray_at_least` | O(n) | O(n) | Prefix sums + monotone deque; handles negative values |
| `count_subarrays_sum` | O(n log n) | O(n) | Prefix frequencies using deterministic `map` |
| `max_subarray_sum` | O(n) | O(1) | Nonempty Kadane, including all-negative input |
| `next_greater` | O(n) | O(n) | Strict next-greater index; monotone stack |
| `sliding_max` | O(n) | O(k) | Deque of candidates for windows of size k |
| `largest_rectangle` | O(n) | O(n) | Monotone stack, nonnegative histogram heights |
| `merge_intervals` | O(n log n) | O(n) | Closed intervals; touching endpoints merge |
| `max_nonoverlapping` | O(n log n) | O(n) | Earliest-finish greedy for nonempty half-open jobs |
| `longest_unique_substring` | O(n) | O(256) | Last occurrence window; bytes, not Unicode code points |
| `subarrays_exactly_k_distinct` | O(n log n) | O(n) | `atMost(k)-atMost(k-1)` using `map` |
| `grid_bfs` | O(RC+sources) | O(RC) | Four-direction multisource BFS, `#` obstacles |
| `three_sum` | O(n^2) | O(n) copy + output | Unique value triples; sorted two pointers |
| `search_rotated` | O(log n) distinct, O(n) worst with duplicates | O(1) | Any matching index, or -1 |
| `kth_two_sorted` | O(log(min(n,m)+1)) | O(1) | 1-based selection from sorted arrays |
| `split_min_largest_sum` | O(n log(sum+1)) | O(1) | Nonnegative array; exactly k nonempty pieces |
| `max_product_subarray` | O(n) | O(1) | Keep both maximum and minimum products |
| `product_except_self` | O(n) | O(1) + O(n) output | All prefix/suffix products must fit |
| `trapped_water` | O(n) | O(1) | Two boundary maxima |
| `sum_subarray_minimums` | O(n) | O(n) | Modular contributions, asymmetric ties |
| `maximal_rectangle` | O(R(C+1)) | O(C) | Binary matrix -> row histograms |
| `merge_k_sorted` | O(k+N log(k+1)) | O(k) + output | N total items in k sorted arrays |
| `meeting_rooms` | O(n log n) | O(n) | Half-open intervals; end events before starts |
| `min_jumps` | O(n) | O(1) | Greedy BFS layers, unreachable -> -1 |
| `first_missing_positive` | O(n) | O(1) | In-place cyclic placement; modifies input |
| `sort_three_values` | O(n) | O(1) | Dutch flag for 0/1/2; modifies input |
| `majority_element` | O(n) | O(1) | Voting + verification; absent -> nullopt |
| `minimum_window` | O(n+m+256) | O(256) | Cover target byte multiplicities |
| `longest_ones_after_flips` | O(n) | O(1) | At most k zero values in a binary window |
| `count_product_less` | O(n) | O(1) | Strictly positive numbers, overflow-safe shrink |
| `count_subarrays_xor` | O(n log n) | O(n) | Prefix-XOR frequencies in deterministic map |
| `longest_increasing_path` | O(R(C+1)) | O(R(C+1)) | Topological grid layers, no recursive DFS |

The nested loops in monotone stacks/deques are linear because each element is inserted and removed at most once. An arbitrary nested loop does not have this guarantee.

### Data structures -- `data_structures.hpp`

| Template | Build / operation TC | Total stored space | Use / restriction |
|---|---|---|---|
| `DSU` | O(n) build; amortized O(alpha(n)) find/union | O(n) | Connectivity, Kruskal; union by size + path compression |
| `Fenwick` | O(n) build; O(log n) add/sum | O(n) | Point-add, range-sum |
| `Fenwick::lower_bound` | O(log n) | Uses existing tree | Prefix order statistic; all point values nonnegative |
| `RangeFenwick` | O(n) build; O(log n) range add/sum | O(n) | Two BITs, initially zero |
| `SegmentTree<T,Merge>` | O(n) build; O(log n) point-set/query | O(n) | Any associative merge + two-sided identity |
| `LazySumTree` | O(n) build; O(log n) range-add/range-sum | O(n) + O(log n) call stack | Add only, not range assignment |
| `SparseMin` | O(n log n) build; O(1) RMQ | O(n log n) | Static array; overlap works for min, not sum |
| `XorTrie` | Amortized O(B) insert, O(B) max-XOR, B=32 | O(nB) worst case | Unsigned 32-bit values, empty query returns `nullopt` |
| `MedianFinder` | O(log n) insert; O(1) median | O(n) | Two heaps, average of middle two for even size |
| `sliding_median` | O(n log(k+1)) total | O(k) + output | Two multisets erase expired values; no unbounded lazy-deletion heap |
| `MinStack` | Amortized O(1) push; O(1) pop/top/min | O(peak size) | Vector capacity remains after pops |
| `LRUCache` | Expected amortized O(1), worst O(capacity) get/put | O(capacity) | Recency list + hash map; no unsafe iterator copying |
| `RandomizedSet` | Expected amortized O(1) insert/erase/sample; worst O(n) hash operations | O(peak size) | Swap-remove, unbiased sampling via caller RNG |

`mean_of_two(a,b)` in `common.hpp` computes an integer-pair average without integer overflow or premature cancellation when `long double` has only double precision. Medians use this helper. Floating-point results still have the platform's normal rounding precision.

For `SegmentTree`, the stated bounds assume constant-sized values and O(1) merge. String concatenation is associative but not O(1), so it does not inherit the numeric segment tree's time bound.

```cpp
auto minimum = [](cp::i64 a, cp::i64 b) { return std::min(a, b); };
cp::SegmentTree<cp::i64, decltype(minimum)> tree(a, cp::INF, minimum);
tree.set(2, 17);
auto answer = tree.query(1, 4); // [1,4)
```

### Graphs -- `graphs.hpp`, `connectivity.hpp`

| Template | TC | SC | Use / restriction |
|---|---|---|---|
| `bfs` | O(V+E+sources) | O(V) | Unweighted, single/multisource; unreachable -1 |
| `restore_path` | O(predecessor chain length) | O(1) excluding output | Predecessor forest; checks reachability before allocating |
| `dfs_order` | O(V+E) | O(V) | Recursive-equivalent adjacency-order preorder using explicit frames |
| `bipartite_coloring` | O(V+E) | O(V) | Undirected, disconnected allowed; odd cycle -> `nullopt` |
| `topological_sort` | O(V+E) | O(V) | Directed DAG; cycle -> `nullopt` |
| `dijkstra` | O(V+E log(E+1)) | O(V+E) | Nonnegative edges; lazy heap includes stale entries |
| `zero_one_bfs` | O(V+E) | O(V+E) | Edge weights exactly 0 or 1 |
| `bellman_ford` | O(VE+V+E) | O(V+E) | Negative edges; marks every vertex affected by a reachable negative cycle |
| `floyd_warshall` | O(V^3) | O(V^2) working/result copy | All pairs; no negative cycles |
| `kruskal` | O(V+E log(E+1)) | O(V+E) | Undirected MST/forest, each edge supplied once |
| `strongly_connected_components` | O(V+E) | O(V+E) | Iterative Kosaraju, topologically ordered component IDs |
| `bridges_and_articulations` | O(V+E) | O(V+E) | Iterative low-link; handles parallel edges and self-loops |
| `TwoSAT` | O(variables+clauses) solve | Same | OR clauses, assignment or `nullopt` |
| `shortest_visit_all` | O((V+E)*2^V) | O(V*2^V) | State BFS with revisits; practical V<=15 |

`Graph` is an adjacency list of integer vertices. `WeightedGraph` stores `{to,weight}`. Add both directions for an undirected adjacency list. `Edge` lists use `{from,to,weight}`. Bellman-Ford's `negative_infinite[v]` flag takes precedence over its provisional numeric distance; an affected vertex has no finite shortest path. Its comment includes a stronger arithmetic bound for repeated negative-cycle relaxations.

For DAG DP, obtain `topological_sort(g)`, then relax transitions in that order. Each vertex and edge is processed once: O(V+E) time and O(V) DP storage for a constant-size state per vertex. Initialize only valid start states; unreachable states must not participate in arithmetic.

### Trees -- `trees.hpp`, `offline.hpp`

| Template | TC | Space | Use / restriction |
|---|---|---|---|
| `BinaryLifting` construction | O(n log n) | O(n log n) stored | Connected nonempty undirected tree |
| `lca`, `distance`, `kth_ancestor` | O(log n) | O(1) query workspace | kth ancestor above root -> -1 |
| `tin`, `tout`, `is_ancestor` | O(1) per lookup/query | Included above | Flatten a subtree to `[tin[v],tout[v])` |
| `tree_diameter` | O(n) | O(n) | Two BFS passes on an unweighted tree |
| `tree_distance_sums` | O(n) | O(n) | Rerooting, all-root sums of edge distances |
| `tree_independent_set` | O(n) | O(n) | Take/skip tree DP, empty set allowed |
| `FunctionalJump` | O(nB) build, O(B) jump | O(nB) stored | One successor per vertex; supports cycles and 64-bit step counts |
| `mo_distinct` | O(n log n + q log(q+1) + (n+q)sqrt(n)) | O(n+q) | Offline static distinct queries, no updates |

Subtree queries combine Euler/preorder indices with a Fenwick/segment tree: put vertex v's value at `flat[tin[v]]`. Query `[tin[v],tout[v])`. Tree LCA and functional-graph jumps have different contracts; do not use the tree constructor on a cyclic graph.

### Dynamic programming -- `dp.hpp`

| Template | TC | SC | State / restriction |
|---|---|---|---|
| `lis_indices` | O(n log n) | O(n) | Strict LIS with reconstruction |
| `knapsack_01` | O(nW) | O(W) | Descending capacities, at most one of each item |
| `knapsack_unbounded` | O(nW) | O(W) | Ascending capacities, unlimited copies |
| `subset_sum` | O(nT) | O(T) | Nonnegative numbers, exact target T |
| `min_coins` | O(nA) | O(A) | Unlimited positive coins; impossible -> -1 |
| `coin_combinations` | O(nA) | O(A) | Distinct denominations, unordered combinations modulo mod |
| `lcs_length` | O(nm) | O(min(n,m)) | Longest common subsequence, not substring |
| `edit_distance` | O(nm) | O(min(n,m)) | Unit-cost insert/delete/replace |
| `weighted_scheduling` | O(n log n) | O(n) | Profit DP + binary search for compatible predecessor |
| `matrix_chain` | O(n^3) | O(n^2) | Interval DP: every split for every interval |
| `traveling_salesman` | O(n^2 * 2^n) | O(n * 2^n) | Held-Karp bitmask DP, n<=20, INF for absent edges |
| `count_digit_sum` | O(D*(S+1)*10) | O(S+1) | Tight digit DP, bound<=10^18, includes 0 |
| `subset_zeta` | O(B*2^B) | O(2^B) working/result vector | SOS DP; vector length is a power of two |
| `nonadjacent_sum` | O(n) | O(1) | Linear/circular House Robber, empty choice allowed |
| `stock_profit_k` | O(n(k+1)); O(n) when k>=n/2 | O(k+1); O(1) unlimited fast path | At most k trades, optional sale fee, no cooldown |
| `stock_profit_cooldown` | O(n) | O(1) | Unlimited trades, one-day cooldown, optional fee |
| `word_break` | O(w+S+n(L+1)) | O(26S+n) | Trie-guided segmentation; w words, S characters, longest word L |
| `distinct_subsequences` | O(nm+n+m) | O(m+1) | Modular count; descending target positions |
| `palindrome_min_cuts` | O(n^2) | O(n^2) | Palindrome table + minimum prefix pieces |
| `burst_balloons` | O(n^3) | O(n^2) | Last-removal interval DP |
| `take_ends_score_difference` | O(n^2) | O(n) | Two-player minimax via score differences |
| `count_unique_digit_numbers` | O(D*1024*10) | O(1024) | Positive numbers only, leading zeros do not occupy digit 0 |
| `wildcard_match` | O(nm+n+m) | O(m+1) | Full-string `?`/`*` matching, not regex |

Knapsack's O(nW) is **pseudo-polynomial**: W is a numeric value, not its input bit length. Knapsack permits taking nothing and optimizes capacity *at most* W. Exact-fill variants need unreachable initialization, not all zeros. `coin_combinations` has coins outside/sums inside; swapping the loops counts ordered sequences instead.

Digit-sum range count `[L,R]` is `count_digit_sum(R,S)-count_digit_sum(L-1,S)` for `0<=L<=R<=10^18`. For digit constraints where leading zeros matter, introduce a `started` flag; for modulo constraints add a remainder state and account for that state count in TC/SC.

### Strings -- `strings.hpp`

| Template | TC | Space | Important detail |
|---|---|---|---|
| `prefix_function` | O(n) | O(n) output, O(1) auxiliary | Borders/failure function |
| `kmp_search` | O(n+m) | O(m) + matches output | Exact matching, overlaps and empty pattern supported |
| `z_function` | O(n) | O(n) output, O(1) auxiliary | `z[0]=0`; prefix-match lengths |
| `Trie` | Amortized O(L) insertion; worst-case O(L) query | O(26 * total inserted characters) worst case | Lowercase a-z, duplicates and prefix counts |
| `RollingHash` | O(n) build, O(1) substring hash | O(n) stored | Double hash, **collisions possible**, compare lengths too |
| `manacher` | O(n) | O(n) output, O(1) auxiliary | Odd/even palindrome radii; sum gives palindrome count |
| `palindrome_table` | O(n^2) | O(n^2) output, O(1) auxiliary | Inclusive `[l,r]` table for partition/cut DP |

`Trie::insert` returns the terminal node ID. `transition(node,ch)` and `terminal_count(node)` are O(1) traversal primitives; the root is node 0, and a missing transition returns -1. They allow word-break DP and multiword board search to reuse the same trie instead of copying implementations.

### Math and combinatorics -- `math.hpp`

| Template | TC | SC / stored space | Restriction |
|---|---|---|---|
| `normalize` | O(1) | O(1) | Positive modulus, handles negative residues |
| `mod_pow` | O(log exponent) | O(1) | Nonnegative exponent, modulus is positive signed int |
| `extended_gcd` | O(log(min(a,b)+1)) | O(1) | Nonnegative operands, not both zero |
| `mod_inverse` | O(log mod) | O(1) | Mod>=2, inverse exists iff gcd=1; handles composite mod |
| `Combinations` | O(N+log p) build, O(1) nCk | O(N) stored | **Prime** p, N<p; does not verify primality |
| `LinearSieve` | O(N) build | O(N) stored | Smallest prime factors + primes |
| `LinearSieve::factorize` | O(log x) | O(1) excluding output | x between 1 and sieve limit |
| `trial_factorize` | O(sqrt x) | O(1) excluding output | Positive x; slow for large primes |
| `totient` | O(sqrt n) | O(log n) factor list | Euler phi for positive n |
| `divisors` | O(sqrt n) | O(number of divisors) | Sorted without an extra sort |
| `checked_lcm` | O(log(min(a,b)+1)) | O(1) | Nonnegative inputs; overflow -> `nullopt` |
| `crt_pair` | O(log(min(m,n)+1)) | O(1) | Two congruences, positive int moduli, inconsistent -> `nullopt` |
| `matrix_multiply` | O(d^3) | O(d^2) result | Square matrices; residues already normalized |
| `matrix_power` | O(d^3 log exponent) | O(d^2) | Linear recurrences, exponent>=0 |

Use `std::gcd` from `<numeric>` for ordinary GCD. Modular division means multiply by an inverse, not integer division followed by `%`. Fermat's inverse `a^(p-2)` requires prime p and a nonzero residue; `mod_inverse` is the safer general interface. This library deliberately does not pretend that 64-bit multiplication supports arbitrary 64-bit moduli.

### Backtracking, bitmasks, divide and conquer -- `search.hpp`

| Template | TC | SC | Important detail |
|---|---|---|---|
| `enumerate_subsets` | O(2^n + callback work) | O(n) | Copies of all outputs require O(n*2^n) additional time/storage |
| `enumerate_permutations` | O(n log n + n*P + callback work), P<=n! | O(n) | Unique lexicographic permutations, duplicate values supported |
| `enumerate_submasks` | O(2^popcount(mask) * callback cost) | O(1) | Includes zero; avoids wraparound infinite loop |
| `subset_sums` | O(2^n) | O(2^n) output | For a small MITM half, including empty subset |
| `count_subsets_sum` | O(n * 2^ceil(n/2)) | O(2^ceil(n/2)) | Meet in the middle, n<=40, negatives allowed |
| `count_inversions` | O(n log n) | O(n) + O(log n) stack | Merge sort, sorts the input in place |
| `combination_sum`, one-use | O(n*2^n + output size) | O(n+h) + output | Positive candidates; duplicate value combinations suppressed |
| `combination_sum`, unlimited | O(n log n + n*C(n+h,h) + output size), conservative | O(n+h) + output | h<=target/min candidate; exponential enumeration, not knapsack DP |
| `enumerate_parentheses` | O(n*C_n + callback work) | O(n) | C_n is Catalan number; valid-prefix pruning |
| `enumerate_palindrome_partitions` | O(n^2+n*2^n+callback work) | O(n^2+n) | Shared palindrome table, copied strings in current path |
| `word_exists` | O(RC*4*3^(L-1)+R), L>0 | O(RC+L) | No cell reuse, input preserved |
| `find_words` | O(S+w+R+RC*4*3^(L-1)+output characters), L>=1 | O(26S+RC+L) | Shared trie pruning; one output per distinct word |
| `enumerate_n_queens` | O(n*n!+callback work), conservative | O(n) | Column/diagonal masks; intended n<=15 |

For `find_words`, S is total dictionary characters, w word count, and L maximum word length. Its empty-dictionary case returns no words. For all callback enumerators, copy outputs you retain and do not modify the working path. Backtracking is inherently exponential and uses the native recursion stack; a huge target with a tiny minimum candidate can overflow that stack even before enumeration becomes practical.

Use unsigned masks: `std::uint64_t{1} << bit` is valid only for `0<=bit<64`. Enumerating submasks for *all* B-bit masks costs O(3^B), not O(2^B). GCC/Clang `__builtin_popcountll(x)` is useful, but `__builtin_clzll(0)` and `__builtin_ctzll(0)` are undefined. `std::popcount` is C++20, not C++17.

### LeetCode nodes -- `nodes.hpp`

| Template | TC | SC | Mutation / restriction |
|---|---|---|---|
| `reverse_list` | O(n) | O(1) | Mutates an acyclic list |
| `cycle_entry` | O(n) | O(1) | Floyd slow/fast pointers, returns entry or nullptr |
| `merge_sorted_lists` | O(n+m) | O(1) | Relinks sorted **disjoint**, acyclic lists |
| `inorder` | O(n) | O(h) + output | Iterative stack |
| `level_order` | O(n) | O(w) + output | BFS by levels |
| `is_bst` | O(n) | O(h) | Strict BST, duplicates forbidden |
| `middle_node` | O(n) | O(1) | Second middle for even lengths |
| `list_intersection` | O(n+m) | O(1) | Acyclic lists, shared-node identity |
| `reverse_k_group` | O(n) | O(1) | Reverses complete groups, leaves short suffix |
| `palindrome_list` | O(n) | O(1) | Temporarily reverses and restores all links |
| `sort_list` | O(n log n) | O(1) | Bottom-up stable mergesort, rewires nodes |
| `binary_tree_fold` | O(n) for constant-time combine/state | O(h) for constant-size state | Generic iterative postorder DP |
| `binary_tree_stats` | O(n) | O(h) | Height, balance, edge diameter, nonempty maximum path sum |
| `binary_tree_lca` | O(n) | O(h) | One-query pointer LCA; absent node -> nullptr |
| `count_tree_path_sum` | O(n log(h+2)) | O(h) | Active-path prefix counts; ordered map guarantees |

These helpers do not own, allocate, or delete nodes. Tree height h can be n for a chain: never automatically write O(log n) for tree recursion/stack space.

### Geometry and stretch graph topics -- `geometry.hpp`, `flow.hpp`

| Template | TC | SC / stored space | Restriction |
|---|---|---|---|
| `cross`, `on_segment`, `segments_intersect` | O(1) | O(1) | Integer coordinates with absolute value <=10^9 |
| `convex_hull` | O(n log n) | O(n) | CCW hull, removes collinear interior boundary points |
| `twice_polygon_area` | O(n) | O(1) | Simple polygon in boundary order, accumulated sum must fit i64 |
| `Dinic::max_flow` | O(V^2 E), general case | O(V+E) stored + O(V) DFS | Integral nonnegative capacities; recursive DFS |
| `Dinic::min_cut_side` | O(V+E) | O(V) | Call after max flow |
| `bipartite_matching` | O(L*(L+E)+R) | O(L+R) including recursion/output | Kuhn; adjacency is left-to-right only |

Dinic mutates its residual network; another call returns additional flow. Use a fresh object for a fresh problem. Self-loops are excluded from this implementation (they cannot improve s-t flow); parallel edges are allowed.

## STL tools to know without reinventing them

| Operation | Time | Caveat |
|---|---|---|
| `sort` on vector | O(n log n) comparisons | Strict weak ordering; never compare with `<=` |
| `lower_bound`, `upper_bound`, `equal_range` on sorted vector | O(log n) | `lower_bound` first >=x, `upper_bound` first >x |
| `set`/`map` insert/find/erase | O(log n) | Use member `lower_bound`; generic version on iterators can take O(n) increments |
| `unordered_map` insert/find | Average O(1), worst O(n) each | Collision-heavy input can make n operations O(n^2) |
| `priority_queue` push/pop/top | O(log n) / O(log n) / O(1) | Max-heap by default; `greater<T>` gives min-heap |
| `make_heap` | O(n) | Building via n pushes instead is O(n log n) |
| `nth_element` | Average O(n) | Not a fully sorted range |
| `vector::push_back` | Amortized O(1) | Reallocation invalidates references/iterators |
| `deque` push/pop at either end | O(1) | Middle insertion/erasure is not O(1) |
| `accumulate` | O(n) | Start with `0LL` for 64-bit accumulation |
| `next_permutation` | O(n) per call | Start sorted to enumerate unique permutations |
| `bitset<N>` AND/OR/shift | Typically O(N / machine_word_bits) | Word-parallel operations, not O(1) for arbitrary N |

For sweep-line overlap/event problems, sort events in O(n log n), define ties to match closed/half-open endpoints, and maintain active state with a counter, heap, or ordered set. For binary-search-on-answer, prove monotonicity before choosing the predicate. Greedy solutions need an exchange/invariant argument; a template alone is not a proof.

## Suggested learning order

1. **Foundations:** STL, integer bounds, sorting, prefix/difference arrays, compression, binary search, two pointers, maps, greedy intervals.
2. **LC essentials:** windows, monotone contributions/deques, greedy layers, heaps, binary search on answers, linked-list rewiring, binary-tree postorder and prefix-path patterns.
3. **Core intermediate:** DSU, shortest paths, topological/state-augmented BFS/DAG DP, stock/robber state machines, LIS, knapsack, subsequence/interval/game DP, trie-guided search, modular arithmetic and sieve.
4. **Expert-oriented repertoire:** Fenwick, segment/lazy trees, sparse tables, Euler tours/LCA, rerooting, KMP/Z/tries, bitmask and digit DP, meet in the middle.
5. **Broader contest coverage:** SCC/low-link/2-SAT, offline Mo, functional graphs, Manacher, SOS DP, geometry; matching/flow as stretch topics.

For each topic: understand the invariant, implement once without copying, compare against brute force on tiny inputs, then solve problems where recognizing the technique is the hard part. Mix timed contests with untimed upsolving. Track your own failed assumptions, not just accepted submissions.

Deliberately outside the core scope: HLD/link-cut trees, persistent trees, FFT/NTT, suffix arrays/automata, Aho-Corasick, min-cost flow, advanced DP optimization (CHT/divide-and-conquer/Knuth), and large-integer primality/factorization. Some hard contest problems require these; no finite intermediate toolkit covers every Guardian-level problem.

Further study: [LeetCode problemset](https://leetcode.com/problemset/), [CP-Algorithms](https://cp-algorithms.com/), [CSES problem set](https://cses.fi/problemset/), [USACO Guide](https://usaco.guide/), [AtCoder Educational DP](https://atcoder.jp/contests/dp/tasks), and the [Codeforces problemset](https://codeforces.com/problemset).

## Review notes

The LC-focused review corrected these concrete issues in the initial version:

| Finding | Correction |
|---|---|
| Marking every DFS sibling before exploring a child could produce a non-DFS preorder | `dfs_order` now uses explicit continuation frames and matches recursive adjacency order |
| An unreachable `restore_path` call allocated a discarded O(V) path despite claiming O(1) auxiliary space | Reachability is checked before output allocation |
| Casting opposite extreme integers before averaging could lose a representable half-unit median on platforms where long double equals double | Both median structures use `mean_of_two`, combining integer halves before conversion |
| Trie insertions understated occasional vector-reallocation cost | Per-insertion bounds now explicitly say amortized; full-build and query bounds remain separate |

The original range/graph/math/DP suite remains intact, with regression cases added. A separate LC-pattern suite compares new implementations to brute-force/reference models, including repeated values, impossible states, negative values where allowed, zero-capacity caches, restored linked-list links, and deep iterative binary-tree traversals. These are useful correctness evidence, not a proof that every possible input or future contest is covered.

## Before submitting

Confirm the input format (`t` or no `t`), indexing, numeric bounds, empty/singleton cases, duplicate behavior, disconnected graphs, negative values, and whether output is a count/value/path. Turn off local debug output and remove all local-header includes. Estimate both time **and memory**: n=20 TSP already needs roughly 160 MiB for its DP table. Do not claim O(1) space while ignoring recursion, heap entries, input copies, or stored outputs.
