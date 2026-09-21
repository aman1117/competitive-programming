// Generator contract: argv[1] is a seed, stdout is ONE valid problem input.
// Keep generated cases small enough for the brute-force reference.
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

int main(int argc, char **argv) {
    if (argc != 2)
        return 1;
    const auto seed = static_cast<std::uint32_t>(std::stoll(argv[1]));
    std::mt19937 rng(seed);
    const int n = static_cast<int>(rng() % 12);
    const int target = static_cast<int>(rng() % 21) - 10;
    std::cout << n << ' ' << target << '\n';
    for (int i = 0; i < n; ++i)
        std::cout << static_cast<int>(rng() % 11) - 5 << ' ';
    std::cout << '\n';
}
