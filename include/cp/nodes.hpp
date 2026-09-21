#pragma once
#include "common.hpp"

namespace cp {
/// Local practice definitions. On LeetCode use the site's node definitions and
/// paste only the needed function (remove the namespace if appropriate).
/// These helpers do not allocate, delete, or own nodes.
struct ListNode {
    int val;
    ListNode* next = nullptr;
    explicit ListNode(int value) : val(value) {}
};
struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    explicit TreeNode(int value) : val(value) {}
};

/// Acyclic list. Mutates links. TC O(n), SC O(1).
inline ListNode* reverse_list(ListNode* head) {
    ListNode* previous = nullptr;
    while (head) {
        ListNode* next = head->next;
        head->next = previous;
        previous = head;
        head = next;
    }
    return previous;
}

/// Floyd cycle detection: returns cycle ENTRY or nullptr.
/// TC O(n), SC O(1). After collision, moving one pointer to the head and
/// advancing both one step makes them meet at the entry.
inline ListNode* cycle_entry(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next; fast = fast->next->next;
        if (slow == fast) {
            slow = head;
            while (slow != fast) { slow = slow->next; fast = fast->next; }
            return slow;
        }
    }
    return nullptr;
}

/// Two sorted, ACYCLIC and DISJOINT lists. Relinks existing nodes (stable).
/// TC O(n+m), SC O(1); shared tails violate the disjointness precondition.
inline ListNode* merge_sorted_lists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) { tail->next = a; a = a->next; }
        else { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

/// Inorder traversal, TC O(n), SC O(h) stack, excluding O(n) output.
/// h can be n for a skewed tree, not always log n.
inline std::vector<int> inorder(TreeNode* root) {
    std::vector<TreeNode*> stack;
    std::vector<int> result;
    while (root || !stack.empty()) {
        while (root) { stack.push_back(root); root = root->left; }
        root = stack.back(); stack.pop_back();
        result.push_back(root->val);
        root = root->right;
    }
    return result;
}

/// Level order, TC O(n), SC O(w) queue, w = maximum width; output O(n).
inline std::vector<std::vector<int>> level_order(TreeNode* root) {
    if (!root) return {};
    std::queue<TreeNode*> q;
    q.push(root);
    std::vector<std::vector<int>> result;
    while (!q.empty()) {
        int size = static_cast<int>(q.size());
        std::vector<int> level;
        while (size--) {
            auto* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(std::move(level));
    }
    return result;
}

/// Strict BST (duplicates forbidden); TC O(n), SC O(h).
inline bool is_bst(TreeNode* root) {
    std::vector<TreeNode*> stack;
    std::optional<int> previous;
    while (root || !stack.empty()) {
        while (root) { stack.push_back(root); root = root->left; }
        root = stack.back(); stack.pop_back();
        if (previous && *previous >= root->val) return false;
        previous = root->val;
        root = root->right;
    }
    return true;
}
/// Second middle for even length, nullptr for empty; acyclic list.
/// TC O(n), SC O(1).
inline ListNode* middle_node(ListNode* head) {
    auto* slow = head;
    auto* fast = head;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow;
}

/// Acyclic lists may share a tail. Pointer identity, not equal values.
/// TC O(n+m), SC O(1): switching heads equalizes total distance walked.
inline ListNode* list_intersection(ListNode* a, ListNode* b) {
    auto* x = a;
    auto* y = b;
    while (x != y) { x = x ? x->next : b; y = y ? y->next : a; }
    return x;
}

/// Reverse complete groups of k nodes; leftover short group is unchanged.
/// Acyclic list, k>=1. TC O(n), SC O(1), mutates links.
inline ListNode* reverse_k_group(ListNode* head, int k) {
    assert(k >= 1);
    ListNode dummy(0);
    dummy.next = head;
    auto* before = &dummy;
    while (true) {
        auto* last = before;
        for (int i = 0; i < k && last; ++i) last = last->next;
        if (!last) break;
        auto* after = last->next;
        auto* previous = after;
        auto* current = before->next;
        auto* old_first = current;
        while (current != after) {
            auto* next = current->next;
            current->next = previous; previous = current; current = next;
        }
        before->next = last;
        before = old_first;
    }
    return dummy.next;
}

/// Acyclic list, TC O(n), SC O(1). Reverses the second half temporarily and
/// RESTORES every original link before returning, including on a mismatch.
inline bool palindrome_list(ListNode* head) {
    if (!head || !head->next) return true;
    auto* middle = head;
    auto* fast = head;
    while (fast->next && fast->next->next) { middle = middle->next; fast = fast->next->next; }
    auto* reversed = reverse_list(middle->next);
    bool equal = true;
    for (auto* left = head, *right = reversed; right; left = left->next, right = right->next)
        if (left->val != right->val) equal = false;
    middle->next = reverse_list(reversed);
    return equal;
}

/// Stable bottom-up merge sort of an acyclic list, mutates links.
/// TC O(n log n), SC O(1): iterative rounds avoid recursive O(log n) frames.
inline ListNode* sort_list(ListNode* head) {
    int n = 0;
    for (auto* node = head; node; node = node->next) ++n;
    auto split = [](ListNode* start, int length) -> ListNode* {
        if (!start) return nullptr;
        while (--length && start->next) start = start->next;
        auto* rest = start->next;
        start->next = nullptr;
        return rest;
    };
    ListNode dummy(0);
    dummy.next = head;
    for (int width = 1; width < n;) {
        auto* tail = &dummy;
        auto* current = dummy.next;
        while (current) {
            auto* left = current;
            auto* right = split(left, width);
            current = split(right, width);
            tail->next = merge_sorted_lists(left, right);
            while (tail->next) tail = tail->next;
        }
        if (width > n / 2) break;
        width *= 2;
    }
    return dummy.next;
}

/// Generic postorder tree DP without native recursion. combine(node,left,right)
/// computes one subtree state; empty is the identity for absent children.
/// With constant-sized/copyable State and O(1) combine: TC O(n), SC O(h).
/// More generally account for State copies and combine's own TC/SC.
template<class State, class Combine>
State binary_tree_fold(TreeNode* root, const State& empty, Combine combine) {
    if (!root) return empty;
    struct Frame { TreeNode* node; int phase; State left, right; };
    std::vector<Frame> stack{{root, 0, empty, empty}};
    while (true) {
        auto& frame = stack.back();
        if (frame.phase == 0) {
            frame.phase = 1;
            if (frame.node->left) stack.push_back({frame.node->left, 0, empty, empty});
        } else if (frame.phase == 1) {
            frame.phase = 2;
            if (frame.node->right) stack.push_back({frame.node->right, 0, empty, empty});
        } else {
            State value = combine(frame.node, frame.left, frame.right);
            stack.pop_back();
            if (stack.empty()) return value;
            auto& parent = stack.back();
            if (parent.phase == 1) parent.left = std::move(value);
            else parent.right = std::move(value);
        }
    }
}

struct BinaryTreeStats {
    int height = 0; // Node count: null tree 0, leaf 1.
    bool balanced = true;
    int diameter = 0; // Edge count, not node count.
    std::optional<i64> max_path_sum; // NONEMPTY path; nullopt for null root.
};
/// Height, height-balance, diameter, and maximum path sum in one postorder.
/// TC O(n), SC O(h). Paths may start/end anywhere, but cannot branch.
inline BinaryTreeStats binary_tree_stats(TreeNode* root) {
    BinaryTreeStats result;
    using State = std::pair<int, i64>; // Height, best nonnegative upward gain.
    auto state = binary_tree_fold(root, State{0, 0}, [&](TreeNode* node, State left, State right) {
        result.balanced = result.balanced && std::abs(left.first - right.first) <= 1;
        result.diameter = std::max(result.diameter, left.first + right.first);
        i64 through = node->val + left.second + right.second;
        if (!result.max_path_sum || through > *result.max_path_sum) result.max_path_sum = through;
        // Only ONE child gain can extend to a parent; two children already form
        // a complete path through this node, considered in "through" above.
        return State{1 + std::max(left.first, right.first),
                     std::max(0LL, node->val + std::max(left.second, right.second))};
    });
    result.height = state.first;
    return result;
}

/// One-query LCA by POINTER identity. Returns nullptr if either node is absent
/// or nullptr; p==q is supported. TC O(n), SC O(h); no preprocessing.
inline TreeNode* binary_tree_lca(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!p || !q) return nullptr;
    TreeNode* answer = nullptr;
    binary_tree_fold(root, 0, [&](TreeNode* node, int left, int right) {
        int found = left | right | (node == p ? 1 : 0) | (node == q ? 2 : 0);
        if (found == 3 && !answer) answer = node;
        return found;
    });
    return answer;
}

/// Count downward ancestor-to-descendant paths of sum=target, any endpoints.
/// TC O(n log(h+2)), SC O(h) using a deterministic map of the ACTIVE path.
/// Enter/exit events undo prefix frequencies: sibling branches cannot mix.
inline i64 count_tree_path_sum(TreeNode* root, i64 target) {
    if (!root) return 0;
    struct Event { TreeNode* node; i64 prefix; bool exit; };
    std::vector<Event> stack{{root, 0, false}};
    std::map<i64, int> frequency{{0, 1}};
    i64 answer = 0;
    while (!stack.empty()) {
        auto [node, prefix, exit] = stack.back(); stack.pop_back();
        if (exit) {
            auto it = frequency.find(prefix);
            assert(it != frequency.end());
            if (--it->second == 0) frequency.erase(it);
            continue;
        }
        prefix += node->val;
        auto it = frequency.find(prefix - target);
        if (it != frequency.end()) answer += it->second;
        ++frequency[prefix];
        stack.push_back({node, prefix, true});
        if (node->right) stack.push_back({node->right, prefix, false});
        if (node->left) stack.push_back({node->left, prefix, false});
    }
    return answer;
}
} // namespace cp
