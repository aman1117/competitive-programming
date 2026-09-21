// Standalone C++17 starter. Add only the headers and algorithms this problem needs.
// Keep these starter facilities available while solve() is still being written.
#include <algorithm> // IWYU pragma: keep
#include <cassert> // IWYU pragma: keep
#include <iostream> // IWYU pragma: keep
#include <numeric> // IWYU pragma: keep
#include <string> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep

using i64 = long long;

/// Solve ONE test case. Write the invariant, TC and SC after deriving the solution.
void solve() {
    // Read input, compute the answer, print it followed by '\n'.
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    constexpr bool multiple_test_cases = true; // Set false when the input has no t.
    int tests = 1;
    if constexpr (multiple_test_cases) {
        if (!(std::cin >> tests))
            return 0;
    }
    while (tests-- > 0)
        solve();
}
