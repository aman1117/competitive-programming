#include "cp/sorting.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::mt19937 random(20260922);
    int cases = 0;
    for (int trial = 0; trial < 350; ++trial) {
        const int n = static_cast<int>(random() % 400);
        std::vector<cp::i64> input(n);
        for (auto &value : input)
            value = static_cast<int>(random() % 51) - 25;
        auto expected = input, merge = input, quick = input;
        std::sort(expected.begin(), expected.end());
        cp::merge_sort(merge);
        cp::quick_sort(quick, random());
        if (merge != expected || quick != expected)
            throw std::runtime_error("Sorting disagrees with std::sort");
        cp::merge_sort(merge, std::greater<cp::i64>{});
        cp::quick_sort(quick, 17, std::greater<cp::i64>{});
        std::reverse(expected.begin(), expected.end());
        if (merge != expected || quick != expected)
            throw std::runtime_error("Custom comparator failed");
        ++cases;
    }
    auto by_key = [](const auto &a, const auto &b) { return a.first < b.first; };
    std::vector<std::pair<int, int>> records{{2, 0}, {1, 1}, {2, 2}, {1, 3}, {2, 4}};
    auto stable = records;
    std::stable_sort(stable.begin(), stable.end(), by_key);
    cp::merge_sort(records, by_key);
    if (records != stable)
        throw std::runtime_error("Merge sort lost stability");
    for (int shape = 0; shape < 4; ++shape) {
        std::vector<int> large(100'000);
        for (int i = 0; i < static_cast<int>(large.size()); ++i)
            large[i] = shape == 0 ? i : shape == 1 ? -i : shape == 2 ? 5 : i % 2;
        cp::quick_sort(large);
        if (!std::is_sorted(large.begin(), large.end()))
            throw std::runtime_error("Large quicksort failed");
    }
    std::vector<cp::i64> limits{std::numeric_limits<cp::i64>::max(), 0,
                                std::numeric_limits<cp::i64>::min()};
    cp::quick_sort(limits);
    if (!std::is_sorted(limits.begin(), limits.end()))
        throw std::runtime_error("Extreme values failed");
    std::cout << "Passed " << cases
              << " randomized sorting cases, stability, large shapes, and extreme values.\n";
}
