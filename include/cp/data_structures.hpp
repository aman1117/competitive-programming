#pragma once
#include "common.hpp"

namespace cp {
class DSU {
    std::vector<int> parent, size_;
    int components_;
public:
    // Build TC/space O(n). Union by size + path compression:
    // find/unite amortized O(alpha(n)), not worst-case O(1).
    explicit DSU(int n) : parent(n), size_(n, 1), components_(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        assert(x >= 0 && x < static_cast<int>(parent.size()));
        while (x != parent[x]) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size_[a] < size_[b]) std::swap(a, b);
        parent[b] = a;
        size_[a] += size_[b];
        --components_;
        return true;
    }
    int size(int x) { return size_[find(x)]; }
    int components() const { return components_; }
};

class Fenwick {
    int n;
    std::vector<i64> bit;
public:
    // Build TC/space O(n). add/prefix/sum TC O(log n), auxiliary SC O(1).
    explicit Fenwick(int count) : n(count), bit(count + 1) {}
    explicit Fenwick(const std::vector<i64>& a) : Fenwick(static_cast<int>(a.size())) {
        // Note 1: Linear build forwards each bucket to its immediate parent.
        for (int i = 1; i <= n; ++i) {
            bit[i] += a[i - 1];
            int p = i + (i & -i);
            if (p <= n) bit[p] += bit[i];
        }
    }
    void add(int index, i64 delta) {
        assert(0 <= index && index < n);
        for (int i = index + 1; i <= n; i += i & -i) bit[i] += delta;
    }
    i64 prefix(int end) const {
        assert(0 <= end && end <= n);
        i64 result = 0;
        for (int i = end; i > 0; i -= i & -i) result += bit[i];
        return result;
    }
    i64 sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return prefix(r) - prefix(l);
    }
    // All point values MUST be nonnegative. Smallest index i for which
    // sum[0,i+1) >= target, or n if total < target; require target > 0.
    // TC O(log n), SC O(1), by binary lifting over Fenwick buckets.
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
    // Range-add + range-sum, initially zero. Build TC/space O(n).
    // Every operation TC O(log n), auxiliary SC O(1).
    explicit RangeFenwick(int count) : n(count), slope(count), intercept(count) {}
    void add(int l, int r, i64 delta) {
        assert(0 <= l && l <= r && r <= n);
        boundary(l, delta);
        boundary(r, -delta);
    }
    i64 prefix(int end) const {
        // Note 2: An update at i contributes delta*(end-i) to the prefix.
        return end * slope.prefix(end) - intercept.prefix(end);
    }
    i64 sum(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return prefix(r) - prefix(l);
    }
};

// Merge must be associative with a two-sided identity (a monoid).
// It need not be commutative. Example: sum/0, min/INF, max/-INF, gcd/0.
// Complexity assumes constant-time merge and constant-size T.
template<class T, class Merge>
class SegmentTree {
    int n, base = 1;
    T identity;
    Merge merge;
    std::vector<T> tree;
public:
    // Build TC/space O(n); query/set TC O(log n), SC O(1).
    SegmentTree(const std::vector<T>& a, T id, Merge op)
        : n(static_cast<int>(a.size())), identity(id), merge(op) {
        while (base < n) base *= 2;
        tree.assign(2 * base, identity);
        std::copy(a.begin(), a.end(), tree.begin() + base);
        for (int i = base - 1; i; --i) tree[i] = merge(tree[i * 2], tree[i * 2 + 1]);
    }
    void set(int i, T value) {
        assert(0 <= i && i < n);
        i += base;
        tree[i] = value;
        while ((i /= 2) > 0) tree[i] = merge(tree[i * 2], tree[i * 2 + 1]);
    }
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
    // Range ADD + range SUM only (not assignment). Build TC/space O(n).
    // Operations TC O(log n), recursion SC O(log n).
    // Note 3: Lazy tags postpone visiting descendants of fully covered nodes.
    // Only the two boundary paths need splitting at each level.
    explicit LazySumTree(const std::vector<i64>& a)
        : n(static_cast<int>(a.size())), tree(4 * std::max(1, n)), lazy(tree.size()) {
        if (n) build(1, 0, n, a);
    }
    void add(int l, int r, i64 delta) {
        assert(0 <= l && l <= r && r <= n);
        if (l < r) add(1, 0, n, l, r, delta);
    }
    i64 sum(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        return l == r ? 0 : sum(1, 0, n, l, r);
    }
};

class SparseMin {
    int n;
    std::vector<int> lg;
    std::vector<std::vector<i64>> table;
public:
    // Static RMQ: build TC/space O(n log n), query TC/SC O(1).
    explicit SparseMin(const std::vector<i64>& a) : n(static_cast<int>(a.size())), lg(n + 1) {
        for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;
        if (!n) return;
        table.assign(lg[n] + 1, std::vector<i64>(n));
        table[0] = a;
        for (int k = 1; k <= lg[n]; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                table[k][i] = std::min(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
    }
    i64 query(int l, int r) const {
        assert(0 <= l && l < r && r <= n);
        int k = lg[r - l];
        // Note 4: Overlapping blocks work for idempotent min/max/gcd, NOT sum.
        return std::min(table[k][l], table[k][r - (1 << k)]);
    }
};

class XorTrie {
    struct Node { std::array<int, 2> next{{-1, -1}}; };
    std::vector<Node> nodes{1};
    int count = 0;
public:
    // Unsigned 32-bit keys. Insert amortized O(B), query worst-case O(B), B=32;
    // total space O(nB). A vector reallocation can make one insert more expensive.
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
    // Returns the maximum XOR VALUE, not the stored key; empty -> nullopt.
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

// Online median with two heaps. insert TC O(log n), median TC O(1),
// total space O(n). Even-size median is the arithmetic mean.
class MedianFinder {
    std::priority_queue<i64> low;
    std::priority_queue<i64, std::vector<i64>, std::greater<i64>> high;
public:
    void insert(i64 x) {
        if (low.empty() || x <= low.top()) low.push(x);
        else high.push(x);
        if (low.size() > high.size() + 1) { high.push(low.top()); low.pop(); }
        if (high.size() > low.size()) { low.push(high.top()); high.pop(); }
    }
    std::optional<long double> median() const {
        if (low.empty()) return std::nullopt;
        if (low.size() > high.size()) return static_cast<long double>(low.top());
        return mean_of_two(low.top(), high.top());
    }
};
// Sliding medians via two multisets: TC O(n log(k+1)), SC O(k) excluding output.
// Unlike lazy-deletion heaps, expired items are actually erased, so storage
// does not grow to O(n). low holds the smaller ceil(window_size/2) values.
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

// Per-entry running minimum, not a sentinel trick that can overflow.
// push amortized O(1); pop/top/minimum O(1); storage O(peak stack size).
// vector capacity is retained after popping; it is not O(current size).
class MinStack {
    std::vector<std::pair<i64, i64>> entries;
public:
    void push(i64 value) {
        entries.emplace_back(value, entries.empty() ? value : std::min(value, entries.back().second));
    }
    std::optional<i64> pop() {
        if (entries.empty()) return std::nullopt;
        i64 value = entries.back().first; entries.pop_back(); return value;
    }
    std::optional<i64> top() const {
        return entries.empty() ? std::nullopt : std::optional<i64>{entries.back().first};
    }
    std::optional<i64> minimum() const {
        return entries.empty() ? std::nullopt : std::optional<i64>{entries.back().second};
    }
};

// Hash map + doubly linked recency list. Expected amortized O(1) get/put,
// worst O(capacity) hash operations; storage O(capacity). get changes recency.
class LRUCache {
    using List = std::list<std::pair<int, i64>>;
    std::size_t capacity;
    List recent;
    std::unordered_map<int, List::iterator> index;
public:
    explicit LRUCache(int count) : capacity(static_cast<std::size_t>(count)) { assert(count >= 0); }
    // Iterators belong to this object's list. Default copying would leave the
    // new index pointing into the OLD list; disable copying/moving explicitly.
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;
    std::optional<i64> get(int key) {
        auto it = index.find(key);
        if (it == index.end()) return std::nullopt;
        recent.splice(recent.begin(), recent, it->second);
        return it->second->second;
    }
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

// Dense vector + value->index map; remove swaps with the last item.
// Insert/remove expected amortized O(1), worst O(n); storage O(peak set size).
// Vector capacity/hash buckets may remain allocated after erasures.
class RandomizedSet {
    std::vector<i64> values;
    std::unordered_map<i64, std::size_t> position;
public:
    bool insert(i64 value) {
        if (position.count(value)) return false;
        position.emplace(value, values.size());
        values.push_back(value);
        return true;
    }
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
    // Uniform choice, expected O(1), no modulo bias; caller supplies RNG.
    std::optional<i64> sample(std::mt19937& generator) const {
        if (values.empty()) return std::nullopt;
        return values[std::uniform_int_distribution<std::size_t>(0, values.size() - 1)(generator)];
    }
    std::size_t size() const { return values.size(); }
};
} // namespace cp
