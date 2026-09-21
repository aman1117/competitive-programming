#pragma once
#include "common.hpp"

namespace cp {
/// Prefix sums: build TC O(n), SC O(1) excluding O(n) output.
/// Query sum [l,r) as prefix[r]-prefix[l]: TC/SC O(1).
inline std::vector<i64> prefix_sums(const std::vector<i64>& a) {
    std::vector<i64> p(a.size() + 1);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) p[i + 1] = p[i] + a[i];
    return p;
}

/// @brief Padded 2D prefix sums for immutable rectangular matrices.
/// Build TC/space O((rows+1)*(cols+1)); rectangle queries TC/SC O(1).
class Prefix2D {
    std::vector<std::vector<i64>> p;
    int rows, cols;
public:
    /// Rectangular matrix. Build TC/space O((rows+1)*(cols+1)), including padding.
    /// This is O(rows*cols) for positive dimensions; zero-width rows also work.
    explicit Prefix2D(const std::vector<std::vector<i64>>& a)
        : rows(static_cast<int>(a.size())),
          cols(rows ? static_cast<int>(a[0].size()) : 0) {
        p.assign(rows + 1, std::vector<i64>(cols + 1));
        for (int r = 0; r < rows; ++r) {
            assert(static_cast<int>(a[r].size()) == cols);
            for (int c = 0; c < cols; ++c)
                p[r + 1][c + 1] = a[r][c] + p[r][c + 1] + p[r + 1][c] - p[r][c];
        }
    }
    /// Rectangle [r1,r2) x [c1,c2). TC/SC O(1).
    i64 sum(int r1, int c1, int r2, int c2) const {
        assert(0 <= r1 && r1 <= r2 && r2 <= rows);
        assert(0 <= c1 && c1 <= c2 && c2 <= cols);
        return p[r2][c2] - p[r1][c2] - p[r2][c1] + p[r1][c1];
    }
};

/// Offline range addition to an initially zero array.
/// TC O(n+q), SC O(n); each update touches two boundaries, then one prefix scan.
inline std::vector<i64> range_additions(
    int n, const std::vector<std::tuple<int, int, i64>>& updates) {
    assert(n >= 0);
    std::vector<i64> diff(n + 1), result(n);
    for (auto [l, r, delta] : updates) {
        assert(0 <= l && l <= r && r <= n);
        diff[l] += delta;
        diff[r] -= delta;
    }
    i64 running = 0;
    for (int i = 0; i < n; ++i) result[i] = (running += diff[i]);
    return result;
}

/// Ranks preserve ordering, not distances. TC O(n log n), SC O(n) for sorted copy.
inline std::vector<int> compress(const std::vector<i64>& a) {
    auto values = a;
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    std::vector<int> rank;
    rank.reserve(a.size());
    for (i64 x : a)
        rank.push_back(static_cast<int>(std::lower_bound(values.begin(), values.end(), x) - values.begin()));
    return rank;
}

/// First true in [lo,hi); returns original hi when none is true.
/// Predicate MUST be false...false,true...true. Require hi-lo to fit i64.
/// TC O(log(hi-lo+1) * predicate_cost), SC O(1) + predicate workspace.
template<class Predicate>
i64 first_true(i64 lo, i64 hi, Predicate feasible) {
    assert(lo <= hi);
    while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (feasible(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/// Two pointers on SORTED input; distinct indices, not necessarily distinct values.
/// TC O(n), SC O(1): each pointer moves at most n times. Pair sums must fit i64.
inline std::optional<std::pair<int, int>> two_sum_sorted(const std::vector<i64>& a, i64 target) {
    int l = 0, r = static_cast<int>(a.size()) - 1;
    while (l < r) {
        i64 sum = a[l] + a[r];
        if (sum == target) return std::pair<int, int>{l, r};
        if (sum < target) ++l;
        else --r;
    }
    return std::nullopt;
}

/// Minimum NONEMPTY length with sum >= target; elements must be nonnegative,
/// target > 0. Returns -1 if impossible. TC O(n), SC O(1).
inline int min_length_nonnegative(const std::vector<i64>& a, i64 target) {
    assert(target > 0);
    i64 sum = 0;
    int l = 0, best = static_cast<int>(a.size()) + 1;
    for (int r = 0; r < static_cast<int>(a.size()); ++r) {
        assert(a[r] >= 0);
        sum += a[r];
        // Note 1: r advances n times in total; l also advances at most n times.
        // This nested loop is therefore O(n), not O(n^2).
        while (sum >= target) {
            best = std::min(best, r - l + 1);
            sum -= a[l++];
        }
    }
    return best > static_cast<int>(a.size()) ? -1 : best;
}

/// Same problem WITH negative values allowed. TC O(n), SC O(n).
/// Monotone prefix sums replace the invalid ordinary sliding-window assumption.
inline int shortest_subarray_at_least(const std::vector<i64>& a, i64 target) {
    auto p = prefix_sums(a);
    std::deque<int> q;
    int best = static_cast<int>(a.size()) + 1;
    for (int i = 0; i < static_cast<int>(p.size()); ++i) {
        while (!q.empty() && p[i] - p[q.front()] >= target) {
            best = std::min(best, i - q.front());
            q.pop_front();
        }
        while (!q.empty() && p[q.back()] >= p[i]) q.pop_back();
        q.push_back(i);
    }
    return best > static_cast<int>(a.size()) ? -1 : best;
}

/// Prefix-frequency counting works with negative numbers too.
/// std::map gives deterministic TC O(n log n), SC O(n).
/// unordered_map can give expected O(n), but adversarial hashing can be O(n^2).
inline i64 count_subarrays_sum(const std::vector<i64>& a, i64 target) {
    std::map<i64, i64> frequency{{0, 1}};
    i64 sum = 0, count = 0;
    for (i64 x : a) {
        sum += x;
        auto it = frequency.find(sum - target);
        if (it != frequency.end()) count += it->second;
        ++frequency[sum];
    }
    return count;
}

/// Kadane: maximum NONEMPTY subarray; empty input -> nullopt.
/// TC O(n), SC O(1). Unlike a zero-initialized answer, handles all-negative arrays.
inline std::optional<i64> max_subarray_sum(const std::vector<i64>& a) {
    if (a.empty()) return std::nullopt;
    i64 ending = a[0], best = a[0];
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        ending = std::max(a[i], ending + a[i]);
        best = std::max(best, ending);
    }
    return best;
}

/// Next STRICTLY greater element's index, or n. TC O(n), SC O(n) stack.
inline std::vector<int> next_greater(const std::vector<i64>& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> result(n, n), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] < a[i]) {
            result[st.back()] = i;
            st.pop_back();
        }
        st.push_back(i);
    }
    return result;
}

/// Window maxima: 1 <= k <= n. TC O(n), SC O(k), excluding O(n-k+1) output.
inline std::vector<i64> sliding_max(const std::vector<i64>& a, int k) {
    assert(k >= 1 && k <= static_cast<int>(a.size()));
    std::deque<int> q;
    std::vector<i64> result;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        if (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i);
        if (i >= k - 1) result.push_back(a[q.front()]);
    }
    return result;
}

/// Largest histogram rectangle; heights >= 0. TC O(n), SC O(n).
/// Note 2: Each index is pushed/popped once. Popping discovers its first smaller
/// right boundary; the new stack top is its smaller left boundary.
inline i64 largest_rectangle(const std::vector<i64>& heights) {
    std::vector<int> st;
    i64 best = 0;
    int n = static_cast<int>(heights.size());
    for (int i = 0; i <= n; ++i) {
        i64 h = i == n ? 0 : heights[i];
        assert(h >= 0);
        while (!st.empty() && heights[st.back()] >= h) {
            i64 height = heights[st.back()];
            st.pop_back();
            int left = st.empty() ? -1 : st.back();
            best = std::max(best, height * (i - left - 1));
        }
        if (i < n) st.push_back(i);
    }
    return best;
}

/// Closed intervals: touching endpoints merge. TC O(n log n), SC O(n) input copy
/// + O(log n) sorting stack, excluding output.
inline std::vector<std::pair<i64, i64>> merge_intervals(std::vector<std::pair<i64, i64>> a) {
    for (auto [l, r] : a) assert(l <= r);
    std::sort(a.begin(), a.end());
    std::vector<std::pair<i64, i64>> result;
    for (auto interval : a) {
        if (result.empty() || result.back().second < interval.first) result.push_back(interval);
        else result.back().second = std::max(result.back().second, interval.second);
    }
    return result;
}

/// Maximum number of compatible [start,end) jobs; require start < end.
/// Greedy earliest finish leaves maximum room for future jobs.
/// TC O(n log n), SC O(n) by-value input copy + O(log n) sorting stack.
inline int max_nonoverlapping(std::vector<std::pair<i64, i64>> jobs) {
    for (auto [l, r] : jobs) assert(l < r);
    std::sort(jobs.begin(), jobs.end(), [](auto a, auto b) { return a.second < b.second; });
    std::optional<i64> end;
    int answer = 0;
    for (auto [l, r] : jobs) if (!end || l >= *end) {
        ++answer;
        end = r;
    }
    return answer;
}
/// Unique sorted triples with sum=target. TC O(n^2), SC O(n) sorted input copy,
/// excluding up to O(n^2) output. Triple sums must fit i64.
inline std::vector<std::array<i64, 3>> three_sum(std::vector<i64> a, i64 target = 0) {
    std::sort(a.begin(), a.end());
    std::vector<std::array<i64, 3>> result;
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i) {
        if (i && a[i] == a[i - 1]) continue;
        int l = i + 1, r = n - 1;
        while (l < r) {
            i64 sum = a[i] + a[l] + a[r];
            if (sum < target) ++l;
            else if (sum > target) --r;
            else {
                result.push_back({a[i], a[l], a[r]});
                i64 left = a[l], right = a[r];
                while (l < r && a[l] == left) ++l;
                while (l < r && a[r] == right) --r;
            }
        }
    }
    return result;
}

/// Rotated nondecreasing array; returns ANY matching index or -1.
/// TC O(log n) with distinct values, worst O(n) with duplicates; SC O(1).
inline int search_rotated(const std::vector<i64>& a, i64 target) {
    int l = 0, r = static_cast<int>(a.size()) - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == target) return m;
        if (a[l] == a[m] && a[m] == a[r]) { ++l; --r; }
        else if (a[l] <= a[m]) {
            if (a[l] <= target && target < a[m]) r = m - 1;
            else l = m + 1;
        } else {
            if (a[m] < target && target <= a[r]) l = m + 1;
            else r = m - 1;
        }
    }
    return -1;
}

/// 1-based kth value in the union of TWO sorted arrays; duplicates retained.
/// Require 1<=k<=n+m, n+m fits int. TC O(log(min(n,m)+1)), SC O(1).
/// Median: select middle one/two values; use mean_of_two for the even case.
inline i64 kth_two_sorted(const std::vector<i64>& a, const std::vector<i64>& b, int k) {
    if (a.size() > b.size()) return kth_two_sorted(b, a, k);
    int n = static_cast<int>(a.size()), m = static_cast<int>(b.size());
    assert(k >= 1 && k <= n + m);
    int lo = std::max(0, k - m), hi = std::min(k, n);
    // Find the smallest number taken from a such that b's left boundary does
    // not exceed a's right boundary. The other partition inequality then holds.
    while (lo < hi) {
        int i = lo + (hi - lo) / 2, j = k - i;
        if (j > 0 && i < n && b[j - 1] > a[i]) lo = i + 1;
        else hi = i;
    }
    int j = k - lo;
    if (lo == 0) return b[j - 1];
    if (j == 0) return a[lo - 1];
    return std::max(a[lo - 1], b[j - 1]);
}

/// Split a NONEMPTY nonnegative array into exactly groups nonempty contiguous
/// pieces, minimizing the largest piece sum. TC O(n log(sum(a)+1)), SC O(1).
inline i64 split_min_largest_sum(const std::vector<i64>& a, int groups) {
    assert(!a.empty() && 1 <= groups && groups <= static_cast<int>(a.size()));
    i64 total = 0, lower = 0;
    for (i64 x : a) { assert(x >= 0); total += x; lower = std::max(lower, x); }
    assert(total < INF);
    return first_true(lower, total + 1, [&](i64 limit) {
        int used = 1;
        i64 sum = 0;
        for (i64 x : a) {
            if (sum + x > limit) { ++used; sum = 0; }
            sum += x;
        }
        // Nonnegativity permits splitting further without increasing the max.
        return used <= groups;
    });
}

/// Maximum product of a NONEMPTY subarray; empty -> nullopt.
/// TC O(n), SC O(1). All intermediate products must fit i64.
inline std::optional<i64> max_product_subarray(const std::vector<i64>& a) {
    if (a.empty()) return std::nullopt;
    i64 low = a[0], high = a[0], answer = a[0];
    for (int i = 1; i < static_cast<int>(a.size()); ++i) {
        i64 x = a[i], left = low * x, right = high * x;
        // Keep the minimum too: multiplying a negative minimum can make a max.
        low = std::min({x, left, right});
        high = std::max({x, left, right});
        answer = std::max(answer, high);
    }
    return answer;
}

/// No division; zeros supported. TC O(n), SC O(1) excluding O(n) output.
/// Every prefix/suffix/product computed must fit i64 (not just final answers).
inline std::vector<i64> product_except_self(const std::vector<i64>& a) {
    std::vector<i64> result(a.size(), 1);
    i64 prefix = 1, suffix = 1;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        result[i] = prefix; prefix *= a[i];
    }
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
        result[i] *= suffix; suffix *= a[i];
    }
    return result;
}

/// Trapping rain water, nonnegative heights. TC O(n), SC O(1).
inline i64 trapped_water(const std::vector<i64>& height) {
    int l = 0, r = static_cast<int>(height.size()) - 1;
    i64 left_max = 0, right_max = 0, water = 0;
    while (l <= r) {
        assert(height[l] >= 0 && height[r] >= 0);
        // The lower boundary determines this side's water regardless of what
        // remains inside; no full prefix/suffix maximum arrays are necessary.
        if (left_max <= right_max) {
            left_max = std::max(left_max, height[l]);
            water += left_max - height[l++];
        } else {
            right_max = std::max(right_max, height[r]);
            water += right_max - height[r--];
        }
    }
    return water;
}

/// Sum of all subarray minimums, modulo a positive int modulus; negatives allowed.
/// TC O(n), SC O(n). Each index is pushed and popped exactly once.
inline i64 sum_subarray_minimums(const std::vector<i64>& a, int mod = MOD) {
    assert(mod > 0);
    int n = static_cast<int>(a.size());
    std::vector<int> stack;
    i64 answer = 0;
    for (int r = 0; r <= n; ++r) {
        while (!stack.empty() && (r == n || a[stack.back()] >= a[r])) {
            int i = stack.back(); stack.pop_back();
            int l = stack.empty() ? -1 : stack.back();
            // Previous STRICTLY smaller, next smaller-OR-EQUAL: ties belong to
            // exactly one occurrence, not both. i owns (i-l)*(r-i) subarrays.
            i64 value = a[i] % mod;
            if (value < 0) value += mod;
            answer = (answer + value * (i - l) % mod * (r - i)) % mod;
        }
        if (r < n) stack.push_back(r);
    }
    return answer;
}

/// Rectangular '0'/'1' matrix. Each row becomes histogram heights.
/// TC O(R*(C+1)), SC O(C), excluding input; handles empty/zero-width matrices.
inline i64 maximal_rectangle(const std::vector<std::string>& matrix) {
    int cols = matrix.empty() ? 0 : static_cast<int>(matrix[0].size());
    std::vector<i64> height(cols);
    i64 answer = 0;
    for (const auto& row : matrix) {
        assert(static_cast<int>(row.size()) == cols);
        for (int c = 0; c < cols; ++c) {
            assert(row[c] == '0' || row[c] == '1');
            height[c] = row[c] == '1' ? height[c] + 1 : 0;
        }
        answer = std::max(answer, largest_rectangle(height));
    }
    return answer;
}

/// Merge k individually sorted arrays with N total items.
/// TC O(k + N log(k+1)), SC O(k) heap excluding O(N) output.
inline std::vector<i64> merge_k_sorted(const std::vector<std::vector<i64>>& arrays) {
    using Entry = std::tuple<i64, int, int>;
    std::vector<Entry> initial;
    for (int i = 0; i < static_cast<int>(arrays.size()); ++i)
        if (!arrays[i].empty()) initial.emplace_back(arrays[i][0], i, 0);
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> heap(
        std::greater<Entry>{}, std::move(initial)); // Linear-time heap construction.
    std::vector<i64> result;
    while (!heap.empty()) {
        auto [value, row, column] = heap.top(); heap.pop();
        result.push_back(value);
        if (column + 1 < static_cast<int>(arrays[row].size()))
            heap.emplace(arrays[row][column + 1], row, column + 1);
    }
    return result;
}

/// Minimum rooms for nonempty half-open [start,end) meetings.
/// TC O(n log n), SC O(n). Ends sort BEFORE starts at equal timestamps.
inline int meeting_rooms(const std::vector<std::pair<i64, i64>>& intervals) {
    std::vector<std::pair<i64, int>> events;
    for (auto [l, r] : intervals) {
        assert(l < r);
        events.emplace_back(l, 1); events.emplace_back(r, -1);
    }
    std::sort(events.begin(), events.end());
    int active = 0, answer = 0;
    for (auto [time, delta] : events) {
        (void)time;
        active += delta; answer = std::max(answer, active);
    }
    return answer;
}
/// Jump Game I/II: minimum forward jumps to the last index; -1 if unreachable.
/// Empty/singleton input -> 0. Nonnegative jump lengths. TC O(n), SC O(1).
/// Each greedy layer is all indices reachable in the current number of jumps.
inline int min_jumps(const std::vector<int>& maximum_jump) {
    int n = static_cast<int>(maximum_jump.size());
    if (n <= 1) return 0;
    int end = 0, farthest = 0, jumps = 0;
    for (int i = 0; i < n - 1; ++i) {
        assert(maximum_jump[i] >= 0);
        farthest = std::max(farthest, static_cast<int>(std::min(n - 1LL, i + 1LL * maximum_jump[i])));
        if (i == end) {
            if (farthest == end) return -1;
            end = farthest; ++jumps;
            if (end == n - 1) return jumps;
        }
    }
    return -1;
}

/// First missing POSITIVE integer, mutates input. TC O(n), SC O(1).
/// Put each in-range x at index x-1; each swap permanently places a value.
/// Duplicate values must stop swapping, otherwise the loop can be infinite.
inline int first_missing_positive(std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    assert(n < std::numeric_limits<int>::max());
    for (int i = 0; i < n; ++i)
        while (a[i] >= 1 && a[i] <= n && a[a[i] - 1] != a[i])
            std::swap(a[i], a[a[i] - 1]);
    for (int i = 0; i < n; ++i) if (a[i] != i + 1) return i + 1;
    return n + 1;
}

/// Dutch national flag; all input values are 0/1/2. Mutates in place.
/// TC O(n), SC O(1). [0,low)=0, [low,mid)=1, [high,n)=2.
inline void sort_three_values(std::vector<int>& a) {
    int low = 0, mid = 0, high = static_cast<int>(a.size());
    while (mid < high) {
        assert(a[mid] >= 0 && a[mid] <= 2);
        if (a[mid] == 0) { std::swap(a[low++], a[mid++]); }
        else if (a[mid] == 1) ++mid;
        else std::swap(a[mid], a[--high]); // Recheck the swapped-in value.
    }
}

/// Boyer-Moore voting with a verification pass. Returns a value occurring
/// STRICTLY more than n/2 times, or nullopt (including empty input).
/// TC O(n), SC O(1); cancellation finds only a candidate, not proof of majority.
inline std::optional<i64> majority_element(const std::vector<i64>& a) {
    i64 candidate = 0;
    int balance = 0;
    for (i64 x : a) {
        if (!balance) candidate = x;
        balance += x == candidate ? 1 : -1;
    }
    return std::count(a.begin(), a.end(), candidate) > static_cast<std::ptrdiff_t>(a.size() / 2)
        ? std::optional<i64>{candidate} : std::nullopt;
}
} // namespace cp
