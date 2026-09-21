// Local Codeforces C++17 working file; expand cp/... includes before submission.
// Starter headers are intentionally kept ready for solve(); trim them if desired.
#include <algorithm> // IWYU pragma: keep
#include <array> // IWYU pragma: keep
#include <cassert> // IWYU pragma: keep
#include <cmath> // IWYU pragma: keep
#include <cstdint> // IWYU pragma: keep
#include <deque> // IWYU pragma: keep
#include <functional> // IWYU pragma: keep
#include <iomanip> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <limits> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <numeric> // IWYU pragma: keep
#include <optional> // IWYU pragma: keep
#include <queue> // IWYU pragma: keep
#include <set> // IWYU pragma: keep
#include <stack> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <tuple> // IWYU pragma: keep
#include <unordered_map> // IWYU pragma: keep
#include <unordered_set> // IWYU pragma: keep
#include <utility> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep
#include "cp/arrays.hpp" // IWYU pragma: keep

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
[[maybe_unused]] constexpr i64 INF = numeric_limits<i64>::max() / 4;
[[maybe_unused]] constexpr int MOD = 1'000'000'007;

#ifdef LOCAL
template<class... Args>
void debug(const Args&... args) {
    ((cerr << args << ' '), ...);
    cerr << '\n';
}
#else
// Debug expressions are not evaluated on the judge.
#define debug(...) ((void)0)
#endif

/// @brief Solve one Codeforces test case; replace this empty starter body.
/// Read input and print the answer here. TC/SC depend on your implementation.
void solve() {
    // Read ONE test case, solve, print the answer.
    // Example: int n; cin >> n; vector<i64> a(n); for (auto& x : a) cin >> x;
    //
    // Write the invariant/recurrence and TC/SC for YOUR solution here.
    // There is no meaningful algorithm complexity until solve() is implemented.
    //
    // Note 1: 1LL * a * b widens BEFORE multiplying ints. Even i64 may overflow;
    // prove bounds from constraints. Avoid #define int long long.
    // Note 2: Use '\n', not endl, to avoid flushing on every output line.
    // For interactive problems, explicitly flush each query and remove cin.tie(nullptr).
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // CF commonly has a leading t; set false for a SINGLE-test problem.
    constexpr bool MULTIPLE_TEST_CASES = true;
    int tests = 1;
    if constexpr (MULTIPLE_TEST_CASES) {
        if (!(cin >> tests)) return 0;
    }
    for (int test = 0; test < tests; ++test) solve();
    return 0;
}
