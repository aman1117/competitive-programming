#include "cp/all.hpp"
#include <iostream>
#include <random>
#include <stdexcept>

using namespace cp;
namespace {
std::mt19937 rng(20260920);
int checks = 0;
void check(bool condition, const char* label) {
    ++checks;
    if (!condition) throw std::runtime_error(label);
}
int random_int(int low, int high) {
    return std::uniform_int_distribution<int>(low, high)(rng);
}
std::string random_string(int length) {
    std::string result;
    while (length--) result += static_cast<char>('a' + random_int(0, 2));
    return result;
}

void test_arrays_and_structures() {
    check(!max_subarray_sum({}), "empty Kadane");
    check(Fenwick(0).prefix(0) == 0, "empty Fenwick");
    check(Fenwick(0).lower_bound(1) == 0, "empty Fenwick selection");
    LazySumTree empty({});
    check(empty.sum(0, 0) == 0, "empty lazy tree");
    empty.add(0, 0, 5);
    check(first_true(0, 100, [](i64 x) { return x * x >= 30; }) == 6, "binary search answer");
    check(first_true(4, 4, [](i64) { return true; }) == 4, "empty search range");
    check(first_true(0, 8, [](i64) { return false; }) == 8, "no true predicate");
    check(first_true(-9, 3, [](i64 x) { return x >= -4; }) == -4, "negative search bounds");
    check(two_sum_sorted({1, 2, 4}, 6) == std::make_optional(std::pair<int, int>{1, 2}), "two sum");
    check(!two_sum_sorted({2}, 4), "two sum distinct indices");
    Prefix2D matrix({{1, 2, 3}, {4, 5, 6}});
    check(matrix.sum(0, 1, 2, 3) == 16 && matrix.sum(1, 0, 1, 2) == 0, "2D prefix");
    check(Prefix2D({}).sum(0, 0, 0, 0) == 0, "empty matrix");
    check(compress({40, -2, 40, 7}) == std::vector<int>({2, 0, 2, 1}), "compression");
    check(range_additions(4, {{0, 3, 2}, {1, 4, -1}, {2, 2, 8}})
        == std::vector<i64>({2, 1, 1, -1}), "difference array");
    check(merge_intervals({{3, 5}, {1, 3}, {9, 9}})
        == std::vector<std::pair<i64, i64>>({{1, 5}, {9, 9}}), "merge intervals");
    check(max_nonoverlapping({{0, 2}, {1, 3}, {2, 4}, {4, 5}}) == 3, "interval greedy");
    DSU dsu(5);
    check(dsu.unite(0, 1) && dsu.unite(1, 2) && !dsu.unite(0, 2), "DSU union");
    check(dsu.size(2) == 3 && dsu.components() == 3, "DSU metadata");
    check(mo_distinct({}, {{0, 0}}) == std::vector<int>{0}, "empty Mo query");
    FunctionalJump cycle({1, 2, 0}, std::numeric_limits<std::uint64_t>::max());
    check(cycle.jump(0, std::numeric_limits<std::uint64_t>::max()) == 0, "64-bit functional jump");

    for (int repetition = 0; repetition < 150; ++repetition) {
        int n = random_int(1, 18);
        std::vector<i64> a(n);
        for (auto& x : a) x = random_int(-6, 9);
        auto prefix = prefix_sums(a);
        std::vector<std::pair<int, int>> queries;
        std::vector<int> distinct_counts;
        for (int i = 0; i < 30; ++i) {
            int l = random_int(0, n), r = random_int(l, n);
            queries.emplace_back(l, r);
            std::set<i64> unique(a.begin() + l, a.begin() + r);
            distinct_counts.push_back(static_cast<int>(unique.size()));
        }
        check(mo_distinct(a, queries) == distinct_counts, "Mo differential");
        std::vector<int> successor(n);
        for (auto& v : successor) v = random_int(0, n - 1);
        FunctionalJump jump(successor, 150);
        int vertex = random_int(0, n - 1), current = vertex;
        for (int steps = 0; steps <= 150; ++steps) {
            check(jump.jump(vertex, steps) == current, "functional jump differential");
            current = successor[current];
        }
        i64 target = random_int(-10, 20), count = 0, best = -INF;
        int shortest = n + 1;
        for (int l = 0; l < n; ++l) {
            i64 sum = 0;
            for (int r = l; r < n; ++r) {
                sum += a[r];
                check(prefix[r + 1] - prefix[l] == sum, "prefix differential");
                count += sum == target;
                best = std::max(best, sum);
                if (sum >= target) shortest = std::min(shortest, r - l + 1);
            }
        }
        check(count_subarrays_sum(a, target) == count, "subarray count differential");
        check(max_subarray_sum(a) == best, "Kadane differential");
        check(shortest_subarray_at_least(a, target) == (shortest == n + 1 ? -1 : shortest), "deque shortest differential");
        auto greater = next_greater(a);
        for (int i = 0; i < n; ++i) {
            int j = i + 1;
            while (j < n && a[j] <= a[i]) ++j;
            check(greater[i] == j, "next greater differential");
        }
        int k = random_int(1, n);
        auto maxima = sliding_max(a, k);
        for (int i = 0; i + k <= n; ++i)
            check(maxima[i] == *std::max_element(a.begin() + i, a.begin() + i + k), "sliding max differential");
        int distinct = random_int(0, 5);
        i64 exact_count = 0;
        for (int l = 0; l < n; ++l) {
            std::set<i64> seen;
            for (int r = l; r < n; ++r) {
                seen.insert(a[r]);
                exact_count += static_cast<int>(seen.size()) == distinct;
            }
        }
        check(subarrays_exactly_k_distinct(a, distinct) == exact_count, "exactly k distinct");

        Fenwick fenwick(a);
        LazySumTree lazy(a);
        auto add = [](i64 x, i64 y) { return x + y; };
        SegmentTree<i64, decltype(add)> segment(a, 0, add);
        SparseMin sparse(a);
        RangeFenwick range(n);
        for (int i = 0; i < n; ++i) range.add(i, i + 1, a[i]);
        for (int step = 0; step < 40; ++step) {
            int l = random_int(0, n), r = random_int(l, n);
            i64 sum = std::accumulate(a.begin() + l, a.begin() + r, 0LL);
            check(fenwick.sum(l, r) == sum && segment.query(l, r) == sum, "point DS query");
            check(lazy.sum(l, r) == sum && range.sum(l, r) == sum, "range DS query");
            if (step == 0 && l < r)
                check(sparse.query(l, r) == *std::min_element(a.begin() + l, a.begin() + r), "sparse min");
            i64 delta = random_int(-5, 5);
            lazy.add(l, r, delta); range.add(l, r, delta);
            for (int i = l; i < r; ++i) {
                a[i] += delta; fenwick.add(i, delta); segment.set(i, a[i]);
            }
        }
        for (auto& x : a) x = random_int(0, 9);
        Fenwick frequencies(a);
        i64 total = std::accumulate(a.begin(), a.end(), 0LL);
        for (i64 t = 1; t <= total + 1; ++t) {
            int position = 0; i64 sum = 0;
            while (position < n && sum + a[position] < t) sum += a[position++];
            check(frequencies.lower_bound(t) == position, "Fenwick selection differential");
        }
        i64 rectangle = 0;
        for (int l = 0; l < n; ++l) {
            i64 height = INF;
            for (int r = l; r < n; ++r) {
                height = std::min(height, a[r]);
                rectangle = std::max(rectangle, height * (r - l + 1));
            }
        }
        check(largest_rectangle(a) == rectangle, "histogram differential");
        target = random_int(1, 40);
        check(min_length_nonnegative(a, target) == shortest_subarray_at_least(a, target), "nonnegative window");
        MedianFinder median;
        std::vector<i64> sorted;
        check(!median.median(), "empty median");
        for (i64 x : a) {
            median.insert(x); sorted.push_back(x); std::sort(sorted.begin(), sorted.end());
            int length = static_cast<int>(sorted.size());
            long double expected = (static_cast<long double>(sorted[(length - 1) / 2]) + sorted[length / 2]) / 2;
            check(median.median() == expected, "median differential");
        }
    }
    // Noncommutative merge checks left/right query ordering.
    auto concat = [](const std::string& a, const std::string& b) { return a + b; };
    SegmentTree<std::string, decltype(concat)> words({"a", "b", "c", "d", "e"}, "", concat);
    check(words.query(1, 5) == "bcde", "segment noncommutative ordering");
}

int component_count(int n, const std::vector<std::pair<int, int>>& edges, int removed_edge = -1, int removed_vertex = -1) {
    DSU dsu(n);
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
        auto [u, v] = edges[i];
        if (i != removed_edge && u != removed_vertex && v != removed_vertex) dsu.unite(u, v);
    }
    return dsu.components() - (removed_vertex != -1);
}

void test_graphs() {
    Graph line{{1}, {0, 2}, {1}, {}};
    auto traversal = bfs(line, {0, 0});
    check(traversal.distance == std::vector<int>({0, 1, 2, -1}), "BFS distances");
    check(restore_path(traversal.parent, 0, 2) == std::vector<int>({0, 1, 2}), "BFS restore");
    check(restore_path(traversal.parent, 0, 3).empty(), "unreachable restore");
    check(dfs_order(line, 0).size() == 3, "iterative DFS");
    check(dfs_order({{1, 2}, {}, {1, 3, 4}, {}, {1}}, 0)
        == std::vector<int>({0, 1, 2, 3, 4}), "DFS must match recursive adjacency-order preorder");
    check(restore_path({-1, 0, -1, 2, 3}, 0, 4).empty(), "path in another predecessor tree");
    check(bfs(line, {0, 2}).distance[1] == 1, "multi-source BFS");
    check(bipartite_coloring(line).has_value(), "bipartite graph");
    check(!bipartite_coloring({{1, 2}, {0, 2}, {0, 1}}), "odd cycle");
    check(topological_sort({{1}, {2}, {}}) == std::make_optional(std::vector<int>{0, 1, 2}), "topological order");
    check(!topological_sort({{1}, {0}}), "topological cycle");
    auto negative = bellman_ford(5, {{0, 1, 1}, {1, 2, -2}, {2, 1, 1}, {2, 3, 4}}, 0);
    check(negative.negative_infinite == std::vector<char>({0, 1, 1, 1, 0}), "negative cycle propagation");
    auto disconnected_cycle = bellman_ford(3, {{1, 2, -1}, {2, 1, -1}}, 0);
    check(disconnected_cycle.distance[1] == INF && !disconnected_cycle.negative_infinite[1], "unreachable negative cycle");
    check(bellman_ford(1, {{0, 0, -1}}, 0).negative_infinite[0], "negative self cycle");
    check(bellman_ford(4, {{0, 1, 5}, {0, 2, 2}, {1, 2, -7}, {2, 3, 1}}, 0).distance
        == std::vector<i64>({0, 5, -2, -1}), "finite negative-edge shortest paths");
    check(kruskal(4, {{0, 1, 2}, {1, 2, 1}, {0, 2, 8}}).components == 2, "spanning forest");

    for (int repetition = 0; repetition < 100; ++repetition) {
        int n = random_int(2, 8);
        WeightedGraph graph(n), binary(n);
        Graph directed(n);
        std::vector<Edge> edges;
        Matrix matrix(n, std::vector<i64>(n, INF));
        for (int v = 0; v < n; ++v) matrix[v][v] = 0;
        for (int v = 0; v < n; ++v) for (int u = 0; u < n; ++u)
            if (v != u && random_int(0, 3) == 0) {
                i64 weight = random_int(0, 9);
                graph[v].push_back({u, weight});
                binary[v].push_back({u, weight % 2});
                directed[v].push_back(u);
                edges.push_back({v, u, weight});
                matrix[v][u] = weight;
            }
        auto all = floyd_warshall(matrix);
        auto scc = strongly_connected_components(directed);
        for (int source = 0; source < n; ++source) {
            auto shortest = dijkstra(graph, source);
            auto bf = bellman_ford(n, edges, source);
            check(shortest.distance == all[source] && bf.distance == all[source], "shortest paths differential");
            check(zero_one_bfs(binary, source) == dijkstra(binary, source).distance, "0-1 BFS differential");
            for (int target = 0; target < n; ++target)
                check((scc.component[source] == scc.component[target])
                    == (all[source][target] != INF && all[target][source] != INF), "SCC differential");
        }
        std::vector<std::pair<int, int>> undirected;
        for (int i = 0, m = random_int(0, 18); i < m; ++i)
            undirected.emplace_back(random_int(0, n - 1), random_int(0, n - 1));
        auto low = bridges_and_articulations(n, undirected);
        int baseline = component_count(n, undirected);
        std::set<int> bridge_ids(low.bridge_ids.begin(), low.bridge_ids.end());
        for (int i = 0; i < static_cast<int>(undirected.size()); ++i)
            check((bridge_ids.count(i) != 0) == (component_count(n, undirected, i) > baseline), "bridge differential");
        for (int v = 0; v < n; ++v)
            check(static_cast<bool>(low.articulation[v]) == (component_count(n, undirected, -1, v) > baseline), "articulation differential");

        int vars = random_int(1, 5);
        TwoSAT sat(vars);
        std::vector<std::tuple<int, bool, int, bool>> clauses;
        for (int i = 0; i < 10; ++i) {
            int x = random_int(0, vars - 1), y = random_int(0, vars - 1);
            bool a = random_int(0, 1) != 0, b = random_int(0, 1) != 0;
            sat.add_or(x, a, y, b); clauses.emplace_back(x, a, y, b);
        }
        auto valid = [&](int mask) {
            for (auto [x, a, y, b] : clauses)
                if ((((mask >> x) & 1) != a) && (((mask >> y) & 1) != b)) return false;
            return true;
        };
        bool exists = false;
        for (int mask = 0; mask < (1 << vars); ++mask) exists = exists || valid(mask);
        auto assignment = sat.solve();
        check(assignment.has_value() == exists, "2-SAT satisfiability");
        if (assignment) {
            int mask = 0;
            for (int v = 0; v < vars; ++v) if ((*assignment)[v]) mask |= 1 << v;
            check(valid(mask), "2-SAT assignment");
        }
    }
    // MST compared to exhaustive spanning trees on a small complete graph.
    for (int repetition = 0; repetition < 30; ++repetition) {
        std::vector<Edge> edges;
        for (int u = 0; u < 4; ++u) for (int v = u + 1; v < 4; ++v)
            edges.push_back({u, v, random_int(-5, 10)});
        i64 best = INF;
        for (int mask = 0; mask < 64; ++mask) {
            DSU dsu(4); i64 weight = 0; int chosen = 0;
            for (int i = 0; i < 6; ++i) if (mask & (1 << i)) {
                dsu.unite(edges[i].from, edges[i].to); weight += edges[i].weight; ++chosen;
            }
            if (chosen == 3 && dsu.components() == 1) best = std::min(best, weight);
        }
        check(kruskal(4, edges).weight == best, "MST exhaustive");
    }
    auto grid = grid_bfs({"..#", ".#.", "..."}, {{0, 0}});
    check(grid[1][2] == 5 && grid[0][2] == -1, "grid BFS");
    check(grid_bfs({}, {}).empty(), "empty grid");
}

void test_trees_and_flow() {
    for (int repetition = 0; repetition < 70; ++repetition) {
        int n = random_int(1, 11);
        Graph tree(n);
        for (int v = 1; v < n; ++v) {
            int p = random_int(0, v - 1);
            tree[v].push_back(p); tree[p].push_back(v);
        }
        int root = random_int(0, n - 1);
        BinaryLifting lift(tree, root);
        auto sums = tree_distance_sums(tree);
        int diameter = 0;
        for (int v = 0; v < n; ++v) {
            auto distances = bfs(tree, {v}).distance;
            check(sums[v] == std::accumulate(distances.begin(), distances.end(), 0LL), "reroot differential");
            for (int u = 0; u < n; ++u) {
                check(lift.distance(v, u) == distances[u], "LCA distance differential");
                diameter = std::max(diameter, distances[u]);
                int a = v, b = u;
                while (lift.depth[a] > lift.depth[b]) a = lift.parent[a];
                while (lift.depth[b] > lift.depth[a]) b = lift.parent[b];
                while (a != b) { a = lift.parent[a]; b = lift.parent[b]; }
                check(lift.lca(v, u) == a, "LCA differential");
                int p = u;
                while (p != root && p != v) p = lift.parent[p];
                check(lift.is_ancestor(v, u) == (p == v), "Euler subtree interval");
            }
            int p = v;
            for (int k = 0; k <= lift.depth[v]; ++k) {
                check(lift.kth_ancestor(v, k) == p, "kth ancestor"); p = lift.parent[p];
            }
            check(lift.kth_ancestor(v, lift.depth[v] + 1) == -1, "above root");
        }
        check(tree_diameter(tree).length == diameter, "diameter differential");
        std::vector<i64> weight(n);
        for (auto& w : weight) w = random_int(-3, 8);
        i64 best = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            bool valid = true; i64 total = 0;
            for (int v = 0; v < n; ++v) if (mask & (1 << v)) {
                total += weight[v];
                for (int u : tree[v]) if (mask & (1 << u)) valid = false;
            }
            if (valid) best = std::max(best, total);
        }
        check(tree_independent_set(tree, weight) == best, "tree DP differential");
    }
    for (int repetition = 0; repetition < 60; ++repetition) {
        int n = random_int(2, 7);
        Dinic flow(n);
        std::vector<Edge> edges;
        for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
            if (u != v && random_int(0, 2) == 0) {
                int cap = random_int(0, 9);
                flow.add_edge(u, v, cap); edges.push_back({u, v, cap});
            }
        i64 min_cut = INF;
        for (int mask = 0; mask < (1 << n); ++mask)
            if ((mask & 1) && !(mask & (1 << (n - 1)))) {
                i64 cut = 0;
                for (auto e : edges) if ((mask & (1 << e.from)) && !(mask & (1 << e.to))) cut += e.weight;
                min_cut = std::min(min_cut, cut);
            }
        check(flow.max_flow(0, n - 1) == min_cut, "flow mincut exhaustive");
        auto side = flow.min_cut_side(0);
        i64 cut = 0;
        for (auto e : edges) if (side[e.from] && !side[e.to]) cut += e.weight;
        check(cut == min_cut && !side[n - 1], "residual cut");
        check(flow.max_flow(0, n - 1) == 0, "additional flow semantics");

        int l = 5, r = 4;
        Graph g(l);
        for (int v = 0; v < l; ++v) for (int u = 0; u < r; ++u)
            if (random_int(0, 1)) g[v].push_back(u);
        auto brute = [&](auto&& self, int v, int used) -> int {
            if (v == l) return 0;
            int best = self(self, v + 1, used);
            for (int u : g[v]) if (!(used & (1 << u)))
                best = std::max(best, 1 + self(self, v + 1, used | (1 << u)));
            return best;
        };
        auto match = bipartite_matching(g, r);
        check(std::count_if(match.begin(), match.end(), [](int x) { return x != -1; }) == brute(brute, 0, 0), "matching differential");
    }
}

void test_strings_and_math() {
    Trie trie;
    trie.insert(""); trie.insert("apple"); trie.insert("app"); trie.insert("app");
    check(trie.count("") == 1 && trie.count("app") == 2 && trie.count("ap") == 0, "trie counts");
    check(trie.prefix_count("ap") == 3 && trie.prefix_count("") == 4 && trie.prefix_count("z") == 0, "trie prefixes");
    XorTrie xor_trie;
    check(!xor_trie.max_xor(0), "empty XOR trie");
    std::vector<std::uint32_t> keys{0U, 1U, 0xffffffffU, 0x80000000U};
    for (auto key : keys) xor_trie.insert(key);
    for (int repetition = 0; repetition < 120; ++repetition) {
        std::uint32_t x = rng();
        keys.push_back(x); xor_trie.insert(x);
        std::uint32_t query = rng(), expected = 0;
        for (auto key : keys) expected = std::max(expected, key ^ query);
        check(xor_trie.max_xor(query) == expected, "XOR trie differential");
        auto text = random_string(random_int(0, 25)), pattern = random_string(random_int(0, 6));
        std::vector<int> occurrences;
        for (int i = 0; i + static_cast<int>(pattern.size()) <= static_cast<int>(text.size()); ++i)
            if (text.compare(i, pattern.size(), pattern) == 0) occurrences.push_back(i);
        check(kmp_search(text, pattern) == occurrences, "KMP differential");
        auto pi = prefix_function(text), z = z_function(text);
        int n = static_cast<int>(text.size());
        for (int i = 0; i < n; ++i) {
            int longest = 0;
            for (int length = 1; length <= i; ++length)
                if (text.substr(0, length) == text.substr(i - length + 1, length)) longest = length;
            check(pi[i] == longest, "prefix function differential");
            int lcp = 0;
            if (i) while (i + lcp < n && text[lcp] == text[i + lcp]) ++lcp;
            check(z[i] == lcp, "Z differential");
        }
        auto radii = manacher(text);
        i64 palindromes = 0;
        int unique_best = 0;
        RollingHash hash(text);
        for (int l = 0; l < n; ++l) {
            std::set<char> characters;
            for (int r = l; r < n; ++r) {
                std::string sub = text.substr(l, r - l + 1), reversed = sub;
                std::reverse(reversed.begin(), reversed.end());
                palindromes += sub == reversed;
                characters.insert(text[r]);
                if (characters.size() == sub.size()) unique_best = std::max(unique_best, r - l + 1);
                RollingHash separate(sub);
                check(hash.hash(l, r + 1) == separate.hash(0, r - l + 1), "substring hash normalization");
            }
        }
        check(palindromes == std::accumulate(radii.odd.begin(), radii.odd.end(), 0LL)
            + std::accumulate(radii.even.begin(), radii.even.end(), 0LL), "Manacher differential");
        check(longest_unique_substring(text) == unique_best, "unique window differential");
    }
    check(normalize(-7, 5) == 3 && mod_pow(2, 10) == 1024 && mod_pow(5, 0, 1) == 0, "modular power");
    check(mod_inverse(3, 8) == 3 && !mod_inverse(2, 8), "composite inverse");
    check(checked_lcm(12, 18) == 36 && checked_lcm(0, 4) == 0, "LCM");
    check(!checked_lcm(std::numeric_limits<i64>::max(), 2), "LCM overflow");
    LinearSieve sieve(500);
    Combinations combinations(30);
    check(Combinations(6, 7).choose(6, 3) == 6, "combinations modular wrap");
    check(mod_pow(-2, 3, 5) == 2, "negative modular base");
    for (int a = 0; a <= 50; ++a) for (int b = 0; b <= 50; ++b) if (a || b) {
        auto bezout = extended_gcd(a, b);
        check(bezout.gcd == std::gcd(a, b) && a * bezout.x + b * bezout.y == bezout.gcd, "Bezout identity");
    }
    std::vector<i64> pascal{1};
    for (int n = 0; n <= 30; ++n) {
        for (int k = 0; k <= n; ++k) check(combinations.choose(n, k) == pascal[k], "combinations Pascal");
        std::vector<i64> next(n + 2, 1);
        for (int k = 1; k <= n; ++k) next[k] = pascal[k - 1] + pascal[k];
        pascal = std::move(next);
    }
    for (int n = 1; n <= 500; ++n) {
        i64 product = 1;
        for (auto [p, exponent] : sieve.factorize(n)) while (exponent--) product *= p;
        check(product == n, "sieve factorization");
        product = 1;
        for (auto [p, exponent] : trial_factorize(n)) while (exponent--) product *= p;
        check(product == n, "trial factorization");
        int phi = 0;
        std::vector<i64> expected;
        for (int k = 1; k <= n; ++k) {
            phi += std::gcd(n, k) == 1;
            if (n % k == 0) expected.push_back(k);
        }
        check(totient(n) == phi && divisors(n) == expected, "totient divisors");
    }
    for (int m = 1; m <= 12; ++m) for (int n = 1; n <= 12; ++n)
        for (int repetition = 0; repetition < 4; ++repetition) {
            int a = random_int(-12, 12), b = random_int(-12, 12);
            auto result = crt_pair(a, m, b, n);
            int lcm = std::lcm(m, n), answer = -1;
            for (int x = 0; x < lcm; ++x)
                if (x % m == normalize(a, m) && x % n == normalize(b, n)) { answer = x; break; }
            check(result.has_value() == (answer != -1), "CRT existence");
            if (result) check(result->first == answer && result->second == lcm, "CRT value");
        }
    check(matrix_power({{1, 1}, {1, 0}}, 10)[0][1] == 55, "matrix Fibonacci");
    check(matrix_power({{2, 3}, {4, 5}}, 0) == Matrix({{1, 0}, {0, 1}}), "matrix identity");
    check(matrix_power({{-1}}, 3, 5)[0][0] == 4, "matrix normalization");
}

void test_dp_and_search() {
    check(lcs_length("abcde", "ace") == 3 && lcs_length("", "abc") == 0, "LCS");
    check(edit_distance("horse", "ros") == 3 && edit_distance("", "ab") == 2, "edit distance");
    check(knapsack_unbounded({2, 3}, {3, 5}, 7) == 11, "unbounded knapsack");
    check(min_coins({1, 3, 4}, 6) == 2 && min_coins({2}, 3) == -1, "minimum coins");
    check(coin_combinations({1, 2, 5}, 5) == 4, "coin combinations");
    check(matrix_chain({40, 20, 30, 10, 30}) == 26000, "interval DP");
    check(matrix_chain({3, 5}) == 0, "single matrix");
    check(!traveling_salesman({{0, 2}, {INF, 0}}), "unreachable TSP");
    check(traveling_salesman({}) == 0 && traveling_salesman({{0}}) == 0, "small TSP");
    check(lis_indices({}).empty(), "empty LIS");
    check(count_digit_sum(-1, 0) == 0 && count_digit_sum(0, 0) == 1, "digit DP zero");
    for (int bound = 0; bound <= 500; bound += 17) {
        std::vector<i64> counts(30);
        for (int x = 0; x <= bound; ++x) {
            int sum = 0;
            for (char digit : std::to_string(x)) sum += digit - '0';
            ++counts[sum];
        }
        for (int sum = 0; sum < 30; ++sum) check(count_digit_sum(bound, sum) == counts[sum], "digit DP exhaustive");
    }
    for (int repetition = 0; repetition < 100; ++repetition) {
        int n = random_int(0, 10), capacity = random_int(0, 15);
        std::vector<i64> a(n), value(n);
        std::vector<int> weight(n);
        for (int i = 0; i < n; ++i) { a[i] = random_int(-4, 7); weight[i] = random_int(1, 7); value[i] = random_int(-3, 9); }
        i64 target = random_int(-5, 12), subset_count = 0, knapsack = 0, inversions = 0;
        int lis_length = 0;
        bool sum_possible = false;
        for (int mask = 0; mask < (1 << n); ++mask) {
            i64 sum = 0, profit = 0, last = -INF;
            int used_weight = 0, length = 0;
            bool increasing = true;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) {
                sum += a[i]; profit += value[i]; used_weight += weight[i]; ++length;
                if (a[i] <= last) increasing = false;
                last = a[i];
            }
            subset_count += sum == target;
            if (used_weight <= capacity) knapsack = std::max(knapsack, profit);
            if (used_weight == capacity) sum_possible = true;
            if (increasing) lis_length = std::max(lis_length, length);
        }
        check(count_subsets_sum(a, target) == subset_count, "MITM exhaustive");
        check(knapsack_01(weight, value, capacity) == knapsack, "knapsack exhaustive");
        check(subset_sum(weight, capacity) == sum_possible, "subset sum exhaustive");
        auto lis = lis_indices(a);
        check(static_cast<int>(lis.size()) == lis_length, "LIS exhaustive");
        for (int i = 1; i < static_cast<int>(lis.size()); ++i)
            check(lis[i - 1] < lis[i] && a[lis[i - 1]] < a[lis[i]], "LIS reconstruction");
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) inversions += a[i] > a[j];
        auto sorted = a;
        check(count_inversions(sorted) == inversions && std::is_sorted(sorted.begin(), sorted.end()), "inversions exhaustive");
        int visits = 0;
        enumerate_subsets(a, [&](const auto&) { ++visits; });
        check(visits == (1 << n), "subset enumeration");
        int bits = random_int(0, 7), states = 1 << bits;
        std::vector<i64> values(states);
        for (auto& x : values) x = random_int(-3, 6);
        auto zeta = subset_zeta(values);
        for (int mask = 0; mask < states; ++mask) {
            i64 sum = 0;
            enumerate_submasks(mask, [&](std::uint64_t sub) { sum += values[sub]; });
            check(zeta[mask] == sum, "SOS differential");
        }
        std::vector<Job> jobs;
        for (int i = 0; i < n; ++i) {
            int start = random_int(0, 8);
            jobs.push_back({start, start + random_int(1, 5), random_int(-3, 10)});
        }
        i64 best = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            i64 profit = 0; bool valid = true;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) {
                profit += jobs[i].profit;
                for (int j = i + 1; j < n; ++j) if ((mask & (1 << j))
                    && std::max(jobs[i].start, jobs[j].start) < std::min(jobs[i].end, jobs[j].end)) valid = false;
            }
            if (valid) best = std::max(best, profit);
        }
        check(weighted_scheduling(jobs) == best, "weighted scheduling exhaustive");
    }
    int permutations = 0;
    enumerate_permutations({1, 1, 2}, [&](const auto&) { ++permutations; });
    check(permutations == 3, "unique permutations");
    for (int repetition = 0; repetition < 20; ++repetition) {
        int n = random_int(2, 7);
        Matrix cost(n, std::vector<i64>(n));
        for (auto& row : cost) for (auto& x : row) x = random_int(0, 20);
        std::vector<int> path(n - 1);
        std::iota(path.begin(), path.end(), 1);
        i64 best = INF;
        do {
            i64 total = cost[0][path[0]] + cost[path.back()][0];
            for (int i = 1; i < n - 1; ++i) total += cost[path[i - 1]][path[i]];
            best = std::min(best, total);
        } while (std::next_permutation(path.begin(), path.end()));
        check(traveling_salesman(cost) == best, "TSP exhaustive");
    }
}

void test_geometry_and_nodes() {
    check(cross({0, 0}, {2, 0}, {1, 1}) == 2, "cross orientation");
    check(segments_intersect({0, 0}, {3, 3}, {0, 3}, {3, 0}), "proper intersection");
    check(segments_intersect({0, 0}, {2, 0}, {1, 0}, {4, 0}), "overlapping segments");
    check(segments_intersect({1, 1}, {1, 1}, {0, 0}, {2, 2}), "point segment");
    check(!segments_intersect({0, 0}, {1, 0}, {2, 0}, {3, 0}), "disjoint segments");
    auto hull = convex_hull({{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}, {1, 0}, {0, 0}});
    check(hull.size() == 4 && twice_polygon_area(hull) == 8, "convex hull area");
    check(convex_hull({{0, 0}, {1, 0}, {2, 0}}).size() == 2, "collinear hull");
    check(convex_hull({}).empty() && twice_polygon_area({}) == 0, "empty geometry");
    check(cross({-1'000'000'000, -1'000'000'000}, {1'000'000'000, -1'000'000'000},
        {-1'000'000'000, 1'000'000'000}) == 4'000'000'000'000'000'000LL, "wide cross product");
    ListNode a(1), b(3), c(2), d(4);
    a.next = &b; c.next = &d;
    auto* list = merge_sorted_lists(&a, &c);
    check(list == &a && a.next == &c && c.next == &b && b.next == &d && !d.next, "merge lists");
    list = reverse_list(list);
    check(list == &d && d.next == &b && b.next == &c && c.next == &a && !a.next, "reverse list");
    check(!cycle_entry(list), "acyclic list");
    a.next = &b;
    check(cycle_entry(list) == &b, "cycle entry");
    a.next = nullptr;
    check(!reverse_list(nullptr), "empty reverse");
    TreeNode root(2), left(1), right(3);
    root.left = &left; root.right = &right;
    check(inorder(&root) == std::vector<int>({1, 2, 3}), "inorder");
    check(level_order(&root) == std::vector<std::vector<int>>({{2}, {1, 3}}), "level order");
    check(is_bst(&root), "valid BST");
    right.val = 1;
    check(!is_bst(&root), "invalid BST");
    check(is_bst(nullptr) && inorder(nullptr).empty() && level_order(nullptr).empty(), "empty binary tree");
}

void test_deep_iterative_traversals() {
    const int n = 100'000;
    Graph directed(n), tree(n);
    std::vector<std::pair<int, int>> edges;
    for (int v = 1; v < n; ++v) {
        directed[v - 1].push_back(v);
        tree[v - 1].push_back(v); tree[v].push_back(v - 1);
        edges.emplace_back(v - 1, v);
    }
    check(strongly_connected_components(directed).count == n, "deep SCC");
    auto low = bridges_and_articulations(n, edges);
    check(static_cast<int>(low.bridge_ids.size()) == n - 1, "deep bridges");
    check(std::count(low.articulation.begin(), low.articulation.end(), true) == n - 2, "deep articulations");
    BinaryLifting lifting(tree);
    check(lifting.distance(0, n - 1) == n - 1 && lifting.kth_ancestor(n - 1, n - 1) == 0, "deep LCA");
    check(tree_distance_sums(tree)[0] == 1LL * n * (n - 1) / 2, "deep reroot");
}
} // namespace

int main() {
    try {
        test_arrays_and_structures();
        test_graphs();
        test_trees_and_flow();
        test_strings_and_math();
        test_dp_and_search();
        test_geometry_and_nodes();
        test_deep_iterative_traversals();
        std::cout << "Passed " << checks << " deterministic/randomized checks (seed 20260920).\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAILED after " << checks << " checks: " << error.what() << '\n';
        return 1;
    }
}
