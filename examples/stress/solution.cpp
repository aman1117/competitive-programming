// Count subarrays with target sum. TC O(n log n), SC O(n).
#include "cp/arrays.hpp"
#include <iostream>

int main() {
    int n;
    cp::i64 target;
    if (!(std::cin >> n >> target))
        return 1;
    std::vector<cp::i64> values(n);
    for (auto &value : values)
        std::cin >> value;
    std::cout << cp::count_subarrays_sum(values, target) << '\n';
}
