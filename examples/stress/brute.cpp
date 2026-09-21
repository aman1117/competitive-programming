// Deliberately simple independent reference: TC O(n^2), SC O(n) for input.
#include <iostream>
#include <vector>

int main() {
    int n;
    long long target;
    if (!(std::cin >> n >> target))
        return 1;
    std::vector<long long> values(n);
    for (auto &value : values)
        std::cin >> value;
    long long answer = 0;
    for (int l = 0; l < n; ++l) {
        long long sum = 0;
        for (int r = l; r < n; ++r) {
            sum += values[r];
            answer += sum == target;
        }
    }
    std::cout << answer << '\n';
}
