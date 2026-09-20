// Input: n q; array; then q commands:
// 1 index delta  -> add delta at ZERO-BASED index
// 2 left right   -> print sum of HALF-OPEN range [left,right)
// TC O(n+q log n), SC O(n). Fenwick builds in O(n).
#include <iostream>
#include "cp/data_structures.hpp"
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    if (!(std::cin >> n >> q)) return 0;
    std::vector<cp::i64> a(n);
    for (auto& x : a) std::cin >> x;
    cp::Fenwick tree(a);
    while (q--) {
        int type, left;
        cp::i64 right;
        std::cin >> type >> left >> right;
        if (type == 1) tree.add(left, right);
        else {
            assert(type == 2 && right <= n);
            std::cout << tree.sum(left, static_cast<int>(right)) << '\n';
        }
    }
}
