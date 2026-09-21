#pragma once
#include "common.hpp"

namespace cp {
/// @brief Stable, bottom-up merge sort; modifies the input without recursion.
/// @note TC O(n log(n+1)); auxiliary SC O(n) for a reusable buffer.
/// Values must be copyable and Compare must be a strict weak ordering.
/// Equal values retain their original order because ties choose the left run.
/// Bounds assume constant-time copies and comparisons.
template <class T, class Compare = std::less<T>>
void merge_sort(std::vector<T> &values, Compare less = Compare{}) {
    const std::size_t n = values.size();
    if (n < 2)
        return;
    std::vector<T> buffer = values;
    for (std::size_t width = 1; width < n;) {
        for (std::size_t left = 0; left < n;) {
            const std::size_t middle = left + std::min(width, n - left);
            const std::size_t right = middle + std::min(width, n - middle);
            std::size_t i = left, j = middle, out = left;
            while (i < middle || j < right) {
                if (j == right || (i < middle && !less(values[j], values[i])))
                    buffer[out++] = values[i++];
                else
                    buffer[out++] = values[j++];
            }
            left = right;
        }
        values.swap(buffer);
        if (width > n / 2)
            break;
        width *= 2;
    }
}

/// @brief In-place randomized three-way quicksort; equal keys share one partition.
/// @param seed Reproducible pivot seed, not a defense against adversarial inputs.
/// @note Expected TC O(n log(n+1)) over pivot choices; worst-case TC O(n^2).
/// Auxiliary SC O(log(n+1)) WORST CASE: recurse only into the smaller partition
/// and handle the larger partition by iteration. Not stable.
/// Values must be copyable; Compare must be a strict weak ordering.
/// Bounds assume constant-time copies/comparisons; the RNG has fixed-size state.
template <class T, class Compare = std::less<T>>
void quick_sort(std::vector<T> &values, std::uint32_t seed = 0xC0FFEEU, Compare less = Compare{}) {
    std::mt19937 generator(seed);
    auto partition_sort = [&](auto &&self, std::size_t left, std::size_t right) -> void {
        while (right - left > 1) {
            T pivot =
                values[std::uniform_int_distribution<std::size_t>(left, right - 1)(generator)];
            std::size_t lower = left, current = left, upper = right;
            // [left,lower)<pivot, [lower,current)==pivot, [upper,right)>pivot.
            while (current < upper) {
                if (less(values[current], pivot))
                    std::swap(values[lower++], values[current++]);
                else if (less(pivot, values[current]))
                    std::swap(values[current], values[--upper]);
                else
                    ++current;
            }
            if (lower - left < right - upper) {
                self(self, left, lower);
                left = upper;
            } else {
                self(self, upper, right);
                right = lower;
            }
        }
    };
    partition_sort(partition_sort, 0, values.size());
}
} // namespace cp
