#pragma once
#include "common.hpp"

namespace cp {
/// @brief Disjoint-set union with union by size and path compression.
/// Build TC/space O(n); find/unite amortized O(alpha(n)).
class DSU {
    std::vector<int> parent, size_;
    int components_;
public:
    /// Build TC/space O(n). Union by size + path compression:
    /// find/unite amortized O(alpha(n)), not worst-case O(1).
    explicit DSU(int n) : parent(n), size_(n, 1), components_(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    /// @brief Return the representative of x's component, compressing its path.
    /// TC amortized O(alpha(n)); auxiliary SC O(1). Requires 0 <= x < n.
    int find(int x) {
        assert(x >= 0 && x < static_cast<int>(parent.size()));
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
    /// @brief Merge two components; return false if they were already connected.
    /// TC amortized O(alpha(n)); auxiliary SC O(1).
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size_[a] < size_[b]) std::swap(a, b);
        parent[b] = a;
        size_[a] += size_[b];
        --components_;
        return true;
    }
    /// @brief Return the number of vertices in x's component.
    /// TC amortized O(alpha(n)); auxiliary SC O(1).
    int size(int x) { return size_[find(x)]; }
    /// @brief Return the current number of disjoint components. TC/SC O(1).
    int components() const { return components_; }
};

/// @brief Fenwick tree for zero-based point additions and half-open range sums.
/// Build TC/space O(n); updates/queries O(log n). Values and sums must fit i64.
class Fenwick {
    int n;
    std::vector<i64> bit;
public:
    /// Build TC/space O(n). add/prefix/sum TC O(log n), auxiliary SC O(1).
    explicit Fenwick(int count) : n(count), bit(count + 1) {}
    /// @brief Build a Fenwick tree from initial values in linear time.
    /// TC O(n); retained SC O(n), forwarding each bucket to its parent once.
    explicit Fenwick(const std::vector<i64>& a) : Fenwick(static_cast<int>(a.size())) {
        // Note 1: Linear build forwards each bucket to its immediate parent.
        for (int i = 1; i <= n; ++i) {
            bit[i] += a[i - 1];
            int p = i + (i & -i);
            if (p <= n) bit[p] += bit[i];
        }
    }
    /// @brief Add delta to a zero-based element; this does not replace its value.
    /// @param index Element index in [0,n).
    /// @param delta Signed amount to add.
    /// @note TC O(log n); auxiliary SC O(1).
    void add(int index, i64 delta) {
        assert(0 <= index && index < n);
        for (int i = index + 1; i <= n; i += i & -i) bit[i] += delta;
    }
    /// @brief Return the sum of the first end elements, range [0,end).
    /// TC O(log n); auxiliary SC O(1). end may be 0 or n.
    i64 prefix(int end) const {
        assert(0 <= end && end <= n);
        i64 result = 0;
        for (int i = end; i > 0; i -= i & -i) result += bit[i];
        return result;
    }
    /// @brief Return the sum of the half-open range [l,r).
    /// TC O(log n); auxiliary SC O(1). An empty range returns zero.
    i64 sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return prefix(r) - prefix(l);
    }
    /// All point values MUST be nonnegative. Smallest index i for which
    /// sum[0,i+1) >= target, or n if total < target; require target > 0.
    /// TC O(log n), SC O(1), by binary lifting over Fenwick buckets.
    int lower_bound(i64 target) const {
        assert(target > 0);
        int pos = 0, step = 1;
        while (step <= n / 2) step *= 2;
        for (; step > 0; step /= 2) {
            int next = pos + step;
            if (next <= n && bit[next] < target) {
                target -= bit[next];
                pos = next;
            }
        }
        return pos;
    }
};

/// @brief Two Fenwick trees supporting range addition and range sums.
/// Initially all zero; retained SC O(n), each operation TC O(log n).
class RangeFenwick {
    int n;
    Fenwick slope, intercept;
    void boundary(int i, i64 delta) {
        if (i < n) {
            slope.add(i, delta);
            intercept.add(i, delta * i);
        }
    }
public:
    /// Range-add + range-sum, initially zero. Build TC/space O(n).
    /// Every operation TC O(log n), auxiliary SC O(1).
    explicit RangeFenwick(int count) : n(count), slope(count), intercept(count) {}
    /// @brief Add delta to every element of [l,r).
    /// TC O(log n); auxiliary SC O(1). Requires 0 <= l <= r <= n.
    void add(int l, int r, i64 delta) {
        assert(0 <= l && l <= r && r <= n);
        boundary(l, delta);
        boundary(r, -delta);
    }
    /// @brief Return the sum of [0,end), combining slope and intercept prefixes.
    /// TC O(log n); auxiliary SC O(1).
    i64 prefix(int end) const {
        // Note 2: An update at i contributes delta*(end-i) to the prefix.
        return end * slope.prefix(end) - intercept.prefix(end);
    }
    /// @brief Return the sum of [l,r) after any range additions.
    /// TC O(log n); auxiliary SC O(1).
    i64 sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return prefix(r) - prefix(l);
    }
};

/// Merge must be associative with a two-sided identity (a monoid).
/// It need not be commutative. Example: sum/0, min/INF, max/-INF, gcd/0.
/// Complexity assumes constant-time merge and constant-size T.
template<class T, class Merge>
class SegmentTree {
    int n, base = 1;
    T identity;
    Merge merge;
    std::vector<T> tree;
public:
    /// Build TC/space O(n); query/set TC O(log n), SC O(1).
    SegmentTree(const std::vector<T>& a, T id, Merge op)
        : n(static_cast<int>(a.size())), identity(id), merge(op) {
        while (base < n) base *= 2;
        tree.assign(2 * base, identity);
        std::copy(a.begin(), a.end(), tree.begin() + base);
        for (int i = base - 1; i; --i) tree[i] = merge(tree[i * 2], tree[i * 2 + 1]);
    }
    /// @brief Replace the value at index i and rebuild its ancestor summaries.
    /// TC O(log n), auxiliary SC O(1), assuming constant-time Merge.
    void set(int i, T value) {
        assert(0 <= i && i < n);
        i += base;
        tree[i] = value;
        while ((i /= 2) > 0) tree[i] = merge(tree[i * 2], tree[i * 2 + 1]);
    }
    /// @brief Merge the half-open range [l,r) in left-to-right order.
    /// TC O(log n), auxiliary SC O(1), assuming constant-time Merge.
    /// An empty range returns the configured identity.
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        T left = identity, right = identity;
        for (l += base, r += base; l < r; l /= 2, r /= 2) {
            if (l & 1) left = merge(left, tree[l++]);
            if (r & 1) right = merge(tree[--r], right);
        }
        return merge(left, right);
    }
};

/// @brief Lazy segment tree for range ADD and range SUM, not range assignment.
/// Build TC/space O(n); each operation TC O(log n), recursion SC O(log n).
class LazySumTree {
    int n;
    std::vector<i64> tree, lazy;
    void build(int v, int l, int r, const std::vector<i64>& a) {
        if (r - l == 1) { tree[v] = a[l]; return; }
        int m = l + (r - l) / 2;
        build(v * 2, l, m, a); build(v * 2 + 1, m, r, a);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
    }
    void apply(int v, int length, i64 delta) {
        tree[v] += delta * length;
        lazy[v] += delta;
    }
    void push(int v, int l, int r) {
        int m = l + (r - l) / 2;
        apply(v * 2, m - l, lazy[v]);
        apply(v * 2 + 1, r - m, lazy[v]);
        lazy[v] = 0;
    }
    void add(int v, int l, int r, int ql, int qr, i64 delta) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) { apply(v, r - l, delta); return; }
        push(v, l, r);
        int m = l + (r - l) / 2;
        add(v * 2, l, m, ql, qr, delta); add(v * 2 + 1, m, r, ql, qr, delta);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
    }
    i64 sum(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return tree[v];
        push(v, l, r);
        int m = l + (r - l) / 2;
        return sum(v * 2, l, m, ql, qr) + sum(v * 2 + 1, m, r, ql, qr);
    }
public:
    /// Range ADD + range SUM only (not assignment). Build TC/space O(n).
    /// Operations TC O(log n), recursion SC O(log n).
    /// Note 3: Lazy tags postpone visiting descendants of fully covered nodes.
    /// Only the two boundary paths need splitting at each level.
    explicit LazySumTree(const std::vector<i64>& a)
        : n(static_cast<int>(a.size())), tree(4 * std::max(1, n)), lazy(tree.size()) {
        if (n) build(1, 0, n, a);
    }
    /// @brief Add delta to every value in [l,r), postponing covered descendants.
    /// TC O(log n); recursion SC O(log n). An empty range has no effect.
    void add(int l, int r, i64 delta) {
        assert(0 <= l && l <= r && r <= n);
        if (l < r) add(1, 0, n, l, r, delta);
    }
    /// @brief Return the sum of [l,r); may push pending lazy tags internally.
    /// TC O(log n); recursion SC O(log n). An empty range returns zero.
    i64 sum(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        return l == r ? 0 : sum(1, 0, n, l, r);
    }
};

/// @brief Immutable range-minimum table using overlapping power-of-two blocks.
/// Build TC/space O(n log n); nonempty range query TC/SC O(1).
class SparseMin {
    int n;
    std::vector<int> lg;
    std::vector<std::vector<i64>> table;
public:
    /// Static RMQ: build TC/space O(n log n), query TC/SC O(1).
    explicit SparseMin(const std::vector<i64>& a) : n(static_cast<int>(a.size())), lg(n + 1) {
        for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
        if (!n) return;
        table.assign(lg[n] + 1, std::vector<i64>(n));
        table[0] = a;
        for (int k = 1; k <= lg[n]; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                table[k][i] = std::min(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
    }
    /// @brief Return the minimum of nonempty [l,r). TC/SC O(1).
    /// Requires 0 <= l < r <= n; overlapping blocks are valid for min, not sum.
    i64 query(int l, int r) const {
        assert(0 <= l && l < r && r <= n);
        int k = lg[r - l];
        // Note 4: Overlapping blocks work for idempotent min/max/gcd, NOT sum.
        return std::min(table[k][l], table[k][r - (1 << k)]);
    }
};

/// @brief Binary trie for maximizing XOR against inserted unsigned 32-bit keys.
/// Insert amortized O(32), query O(32); retained SC O(number_of_keys * 32).
class XorTrie {
    struct Node { std::array<int, 2> next{{-1, -1}}; };
    std::vector<Node> nodes{1};
    int count = 0;
public:
    /// Unsigned 32-bit keys. Insert amortized O(B), query worst-case O(B), B=32;
    /// total space O(nB). A vector reallocation can make one insert more expensive.
    void insert(std::uint32_t value) {
        int v = 0;
        for (int b = 31; b >= 0; --b) {
            int bit = static_cast<int>((value >> b) & 1U);
            if (nodes[v].next[bit] == -1) {
                int child = static_cast<int>(nodes.size());
                nodes[v].next[bit] = child;
                nodes.emplace_back();
            }
            v = nodes[v].next[bit];
        }
        ++count;
    }
    /// Returns the maximum XOR VALUE, not the stored key; empty -> nullopt.
    /// TC O(32), auxiliary SC O(1).
    std::optional<std::uint32_t> max_xor(std::uint32_t value) const {
        if (!count) return std::nullopt;
        int v = 0;
        std::uint32_t result = 0;
        for (int b = 31; b >= 0; --b) {
            int bit = static_cast<int>((value >> b) & 1U);
            if (nodes[v].next[bit ^ 1] != -1) {
                result |= std::uint32_t{1} << b;
                bit ^= 1;
            }
            v = nodes[v].next[bit];
        }
        return result;
    }
};

/// Online median with two heaps. insert amortized TC O(log n), median TC O(1),
/// total space O(n). Even-size median is the arithmetic mean.
class MedianFinder {
    std::priority_queue<i64> low;
    std::priority_queue<i64, std::vector<i64>, std::greater<i64>> high;
public:
    /// @brief Insert a value and rebalance the lower and upper heaps.
    /// TC amortized O(log n); total retained SC O(n).
    void insert(i64 x) {
        if (low.empty() || x <= low.top()) low.push(x);
        else high.push(x);
        if (low.size() > high.size() + 1) { high.push(low.top()); low.pop(); }
        if (high.size() > low.size()) { low.push(high.top()); high.pop(); }
    }
    /// @brief Return the current median, or nullopt when no values were inserted.
    /// For even size, average the middle pair safely. TC/SC O(1).
    std::optional<long double> median() const {
        if (low.empty()) return std::nullopt;
        if (low.size() > high.size()) return static_cast<long double>(low.top());
        return mean_of_two(low.top(), high.top());
    }
};
/// Sliding medians via two multisets: TC O(n log(k+1)), SC O(k) excluding output.
/// Unlike lazy-deletion heaps, expired items are actually erased, so storage
/// does not grow to O(n). low holds the smaller ceil(window_size/2) values.
inline std::vector<long double> sliding_median(const std::vector<i64>& a, int k) {
    assert(1 <= k && k <= static_cast<int>(a.size()));
    std::multiset<i64> low, high;
    auto balance = [&] {
        while (low.size() > high.size() + 1) {
            auto it = std::prev(low.end()); high.insert(*it); low.erase(it);
        }
        while (low.size() < high.size()) {
            auto it = high.begin(); low.insert(*it); high.erase(it);
        }
    };
    std::vector<long double> result;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        if (low.empty() || a[i] <= *low.rbegin()) low.insert(a[i]);
        else high.insert(a[i]);
        balance();
        if (i >= k) {
            auto it = low.find(a[i - k]);
            if (it != low.end()) low.erase(it);
            else { auto old = high.find(a[i - k]); assert(old != high.end()); high.erase(old); }
            balance();
        }
        if (i + 1 >= k) {
            long double value = static_cast<long double>(*low.rbegin());
            if (k % 2 == 0) value = mean_of_two(*low.rbegin(), *high.begin());
            result.push_back(value);
        }
    }
    return result;
}

/// Per-entry running minimum, not a sentinel trick that can overflow.
/// push amortized O(1); pop/top/minimum O(1); storage O(peak stack size).
/// vector capacity is retained after popping; it is not O(current size).
class MinStack {
    std::vector<std::pair<i64, i64>> entries;
public:
    /// @brief Push a value together with the new running minimum.
    /// TC amortized O(1); retained SC O(peak stack size).
    void push(i64 value) {
        entries.emplace_back(value, entries.empty() ? value : std::min(value, entries.back().second));
    }
    /// @brief Remove and return the top value, or nullopt if empty. TC/SC O(1).
    std::optional<i64> pop() {
        if (entries.empty()) return std::nullopt;
        i64 value = entries.back().first; entries.pop_back(); return value;
    }
    /// @brief Read the top value without removing it; empty -> nullopt. TC/SC O(1).
    std::optional<i64> top() const {
        return entries.empty() ? std::nullopt : std::optional<i64>{entries.back().first};
    }
    /// @brief Return the current minimum; empty -> nullopt. TC/SC O(1).
    std::optional<i64> minimum() const {
        return entries.empty() ? std::nullopt : std::optional<i64>{entries.back().second};
    }
};

/// Hash map + doubly linked recency list. Expected amortized O(1) get/put,
/// worst O(capacity) hash operations; storage O(capacity). get changes recency.
class LRUCache {
    using List = std::list<std::pair<int, i64>>;
    std::size_t capacity;
    List recent;
    std::unordered_map<int, List::iterator> index;
public:
    /// @brief Create an empty cache with a nonnegative key capacity.
    /// Zero capacity retains nothing. Initial TC/SC O(1).
    explicit LRUCache(int count) : capacity(static_cast<std::size_t>(count)) { assert(count >= 0); }
    /// Iterators belong to this object's list. Default copying would leave the
    /// new index pointing into the OLD list; disable copying/moving explicitly.
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;
    /// @brief Find a value and mark its key most recently used; absent -> nullopt.
    /// Expected TC O(1), worst O(capacity); auxiliary SC O(1).
    std::optional<i64> get(int key) {
        auto it = index.find(key);
        if (it == index.end()) return std::nullopt;
        recent.splice(recent.begin(), recent, it->second);
        return it->second->second;
    }
    /// @brief Insert/update a value and evict the least-recently-used key if full.
    /// Expected amortized TC O(1), worst O(capacity); retained SC O(capacity).
    void put(int key, i64 value) {
        if (capacity == 0) return; // Defined zero-capacity cache: nothing retained.
        auto it = index.find(key);
        if (it != index.end()) {
            it->second->second = value;
            recent.splice(recent.begin(), recent, it->second);
            return;
        }
        recent.emplace_front(key, value);
        index.emplace(key, recent.begin());
        if (recent.size() > capacity) {
            index.erase(recent.back().first);
            recent.pop_back();
        }
    }
};

/// Dense vector + value->index map; remove swaps with the last item.
/// Insert/remove expected amortized O(1), worst O(n); storage O(peak set size).
/// Vector capacity/hash buckets may remain allocated after erasures.
class RandomizedSet {
    std::vector<i64> values;
    std::unordered_map<i64, std::size_t> position;
public:
    /// @brief Insert a distinct value; return false if it already exists.
    /// Expected amortized TC O(1), worst O(n); retained SC O(peak set size).
    bool insert(i64 value) {
        if (position.count(value)) return false;
        position.emplace(value, values.size());
        values.push_back(value);
        return true;
    }
    /// @brief Erase a value by swapping in the last element; false if absent.
    /// Expected TC O(1), worst O(n); element order is not preserved.
    bool erase(i64 value) {
        auto it = position.find(value);
        if (it == position.end()) return false;
        std::size_t i = it->second;
        i64 last = values.back();
        values[i] = last;
        position.at(last) = i;
        position.erase(it); values.pop_back();
        return true;
    }
    /// Uniform choice, expected O(1), no modulo bias; caller supplies RNG.
    std::optional<i64> sample(std::mt19937& generator) const {
        if (values.empty()) return std::nullopt;
        return values[std::uniform_int_distribution<std::size_t>(0, values.size() - 1)(generator)];
    }
    /// @brief Return the number of currently stored values. TC/SC O(1).
    std::size_t size() const { return values.size(); }
};
} // namespace cp
