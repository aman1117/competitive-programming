#include "cp/all.hpp"
#include <iostream>

int main() {
    // Hover over the type or these calls to see their purpose and complexity.
    cp::Fenwick bit(std::vector<cp::i64>{1, 2, 3});
    bit.add(1, 5);
    std::cout << bit.sum(0, 3) << '\n'; // 11

    std::vector<cp::i64> values{3, 1, 2};
    std::cout << cp::count_inversions(values) << '\n'; // 2; values become sorted.

    cp::DSU components(3);
    components.unite(0, 1);
    assert(components.size(0) == 2);
}
