#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cp {
using i64 = long long;
/// Note 1: INF leaves headroom for additions. All finite costs, sums and DP
/// transitions in this library must fit strictly between -INF and INF.
constexpr i64 INF = std::numeric_limits<i64>::max() / 4;
constexpr int MOD = 1'000'000'007;
/// TC/SC O(1). Add integer halves before converting: converting LLONG_MAX and
/// LLONG_MIN first can incorrectly cancel to zero when long double == double.
/// Final results still have the platform's ordinary floating-point precision.
inline long double mean_of_two(i64 a, i64 b) {
    return static_cast<long double>(a / 2 + b / 2) + (a % 2 + b % 2) / 2.0L;
}
// Note 2: Indices are zero-based, ranges are [l,r), and sizes fit int.
// TC = time complexity; SC = auxiliary space (including recursion), excluding
// inputs and returned outputs unless explicitly stated. Stored DS space is total.
} // namespace cp
