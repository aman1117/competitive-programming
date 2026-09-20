// LeetCode submission starter. Replace method name/signature with the problem's.
// Do NOT submit main() or redefine ListNode/TreeNode when the judge supplies them.
#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution {
public:
    // Compile-ready EXAMPLE, not a universal LeetCode signature.
    // Sum n integers: TC O(n) for one scan, SC O(1) beyond the input.
    long long exampleSum(const vector<int>& nums) {
        return accumulate(nums.begin(), nums.end(), 0LL);
    }
};

// cprun .\templates\leetcode.cpp -Local builds this local-only driver.
// Update these sample calls whenever you replace the example method.
// LeetCode does not define LOCAL, so its judge supplies main().
#ifdef LOCAL
#include <cassert>
#include <iostream>
int main() {
    Solution solution;
    const vector<int> nums{1, 2, 3};
    assert(solution.exampleSum(nums) == 6);
    assert(solution.exampleSum({}) == 0);
    cout << solution.exampleSum(nums) << '\n';
}
#endif
