// Codeforces C++17 starter: this file is standalone and submit-ready.
// Add only the algorithm code you need; judges cannot see your local headers.
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

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
