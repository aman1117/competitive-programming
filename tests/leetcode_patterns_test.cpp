#include "cp/all.hpp"
#include <iostream>
#include <stdexcept>
#include <type_traits>

using namespace cp;
namespace {
std::mt19937 rng(20260921);
int checks = 0;
void check(bool condition, const char* label) {
    ++checks;
    if (!condition) throw std::runtime_error(label);
}
int random_int(int l, int r) { return std::uniform_int_distribution<int>(l, r)(rng); }
std::string text(int length) {
    std::string s;
    while (length--) s += static_cast<char>('a' + random_int(0, 2));
    return s;
}
bool palindrome(std::string s) {
    auto reversed = s;
    std::reverse(reversed.begin(), reversed.end());
    return s == reversed;
}

void test_arrays() {
    for (int rep = 0; rep < 150; ++rep) {
        int n = random_int(0, 10);
        std::vector<i64> a(n), b(random_int(0, 8));
        for (auto& x : a) x = random_int(-3, 5);
        for (auto& x : b) x = random_int(-3, 5);
        std::optional<i64> majority;
        for (i64 x : a) if (std::count(a.begin(), a.end(), x) > n / 2) majority = x;
        check(majority_element(a) == majority, "majority with verification");
        std::vector<int> placed(a.begin(), a.end());
        int missing = 1;
        while (std::find(placed.begin(), placed.end(), missing) != placed.end()) ++missing;
        check(first_missing_positive(placed) == missing, "cyclic placement missing positive");
        std::vector<int> colors(n);
        for (auto& x : colors) x = random_int(0, 2);
        auto expected_colors = colors;
        std::sort(expected_colors.begin(), expected_colors.end()); sort_three_values(colors);
        check(colors == expected_colors, "Dutch flag differential");
        std::vector<int> jumps(n), jump_distance(n, n + 1);
        for (auto& x : jumps) x = random_int(0, 6);
        if (n) jump_distance[0] = 0;
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n && j <= i + jumps[i]; ++j)
                jump_distance[j] = std::min(jump_distance[j], jump_distance[i] + 1);
        check(min_jumps(jumps) == (n == 0 ? 0 : (jump_distance.back() > n ? -1 : jump_distance.back())),
            "greedy jump layers vs DP");
        i64 target = random_int(-4, 8);
        std::set<std::array<i64, 3>> triples;
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
            for (int k = j + 1; k < n; ++k) if (a[i] + a[j] + a[k] == target) {
                std::array<i64, 3> triple{a[i], a[j], a[k]};
                std::sort(triple.begin(), triple.end()); triples.insert(triple);
            }
        auto actual_triples = three_sum(a, target);
        check(std::set<std::array<i64, 3>>(actual_triples.begin(), actual_triples.end()) == triples
            && actual_triples.size() == triples.size(), "three sum exhaustive");
        std::vector<i64> expected(n, 1);
        std::optional<i64> max_product;
        i64 minimum_sum = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) if (i != j) expected[i] *= a[j];
            i64 product = 1, minimum = INF;
            for (int j = i; j < n; ++j) {
                product *= a[j]; minimum = std::min(minimum, a[j]); minimum_sum += minimum;
                if (!max_product || product > *max_product) max_product = product;
            }
        }
        check(product_except_self(a) == expected, "prefix suffix products");
        check(max_product_subarray(a) == max_product, "maximum product exhaustive");
        int mod = random_int(1, 31);
        check(sum_subarray_minimums(a, mod) == normalize(minimum_sum, mod), "minimum contributions ties");
        auto sorted = a;
        std::sort(sorted.begin(), sorted.end()); std::sort(b.begin(), b.end());
        auto merged = sorted; merged.insert(merged.end(), b.begin(), b.end()); std::sort(merged.begin(), merged.end());
        for (int k = 1; k <= static_cast<int>(merged.size()); ++k)
            check(kth_two_sorted(sorted, b, k) == merged[k - 1], "two-array kth partition");
        auto rotated = sorted;
        if (n) std::rotate(rotated.begin(), rotated.begin() + random_int(0, n - 1), rotated.end());
        for (i64 value = -4; value <= 6; ++value) {
            int found = search_rotated(rotated, value);
            bool exists = std::find(a.begin(), a.end(), value) != a.end();
            check(exists ? (found >= 0 && found < n && rotated[found] == value) : found == -1, "rotated duplicates");
        }
        if (n) {
            int k = random_int(1, n);
            auto medians = sliding_median(a, k);
            for (int l = 0; l + k <= n; ++l) {
                std::vector<i64> window(a.begin() + l, a.begin() + l + k);
                std::sort(window.begin(), window.end());
                long double median = (static_cast<long double>(window[(k - 1) / 2]) + window[k / 2]) / 2;
                check(medians[l] == median, "sliding medians differential");
            }
        }
        for (auto& x : a) x = random_int(0, 7);
        i64 water = 0;
        for (int i = 0; i < n; ++i) {
            i64 left = *std::max_element(a.begin(), a.begin() + i + 1);
            i64 right = *std::max_element(a.begin() + i, a.end());
            water += std::min(left, right) - a[i];
        }
        check(trapped_water(a) == water, "rainwater differential");
        if (n) {
            int groups = random_int(1, n);
            i64 best = INF;
            for (int mask = 0; mask < (1 << (n - 1)); ++mask) {
                int pieces = 1;
                i64 sum = 0, maximum = 0;
                for (int i = 0; i < n; ++i) {
                    sum += a[i];
                    if (i == n - 1 || (mask & (1 << i))) {
                        maximum = std::max(maximum, sum); sum = 0;
                        if (i < n - 1) ++pieces;
                    }
                }
                if (pieces == groups) best = std::min(best, maximum);
            }
            check(split_min_largest_sum(a, groups) == best, "binary answer split exhaustive");
        }
        std::vector<std::vector<i64>> arrays(random_int(0, 6));
        std::vector<i64> all;
        for (auto& row : arrays) {
            row.resize(random_int(0, 5));
            for (auto& x : row) x = random_int(-4, 9);
            std::sort(row.begin(), row.end()); all.insert(all.end(), row.begin(), row.end());
        }
        std::sort(all.begin(), all.end());
        check(merge_k_sorted(arrays) == all, "k-way merge");
        std::vector<std::pair<i64, i64>> meetings;
        for (int i = 0; i < n; ++i) {
            int start = random_int(0, 8); meetings.emplace_back(start, start + random_int(1, 5));
        }
        int rooms = 0;
        for (int t = 0; t <= 13; ++t) {
            int active = 0;
            for (auto [l, r] : meetings) active += l <= t && t < r;
            rooms = std::max(rooms, active);
        }
        check(meeting_rooms(meetings) == rooms, "meeting rooms tie order");
    }
    auto limits = std::vector<i64>{std::numeric_limits<i64>::min(), std::numeric_limits<i64>::max()};
    check(kth_two_sorted({}, limits, 1) == limits[0], "kth integer endpoints");
    check(sliding_median(limits, 2)[0] == -0.5L, "median no integer overflow");
    MedianFinder stream;
    stream.insert(limits[0]); stream.insert(limits[1]);
    check(stream.median() == -0.5L, "streaming median cancellation regression");
    check(mean_of_two(-3, -4) == -3.5L && mean_of_two(-3, 4) == 0.5L, "signed remainder averaging");
}

void test_windows_and_grids() {
    check(count_product_less({std::numeric_limits<i64>::max() / 2 + 1, 2},
        std::numeric_limits<i64>::max()) == 2, "product multiplication overflow guard");
    for (int rep = 0; rep < 120; ++rep) {
        int n = random_int(0, 10);
        std::string s = text(n), target = text(random_int(0, 4));
        if (rep % 3 == 0) {
            for (char& ch : s) if (ch == 'a') ch = static_cast<char>(255);
            for (char& ch : target) if (ch == 'a') ch = static_cast<char>(255);
        }
        std::optional<std::pair<int, int>> best;
        if (target.empty()) best = {0, 0};
        else {
            std::array<int, 256> need{};
            for (unsigned char ch : target) ++need[ch];
            for (int l = 0; l < n; ++l) {
                std::array<int, 256> count{};
                for (int r = l; r < n; ++r) {
                    ++count[static_cast<unsigned char>(s[r])];
                    bool covered = true;
                    for (int c = 0; c < 256; ++c) if (count[c] < need[c]) covered = false;
                    if (covered && (!best || r + 1 - l < best->second - best->first)) best = {l, r + 1};
                }
            }
        }
        check(minimum_window(s, target) == best, "minimum window bytes/multiplicity");
        std::vector<int> binary(n);
        std::vector<i64> positive(n);
        std::vector<std::uint32_t> values(n);
        for (int i = 0; i < n; ++i) {
            binary[i] = random_int(0, 1); positive[i] = random_int(1, 5); values[i] = random_int(0, 7);
        }
        int flips = random_int(0, n + 1), longest = 0, limit = random_int(0, 30);
        auto xor_target = static_cast<std::uint32_t>(random_int(0, 15));
        i64 products = 0, xors = 0;
        for (int l = 0; l < n; ++l) {
            int zeros = 0; i64 product = 1; std::uint32_t x = 0;
            for (int r = l; r < n; ++r) {
                zeros += binary[r] == 0;
                if (zeros <= flips) longest = std::max(longest, r - l + 1);
                product *= positive[r]; products += product < limit;
                x ^= values[r]; xors += x == xor_target;
            }
        }
        check(longest_ones_after_flips(binary, flips) == longest, "at most k window");
        check(count_product_less(positive, limit) == products, "positive product count");
        check(count_subarrays_xor(values, xor_target) == xors, "prefix XOR count");
        int rows = random_int(0, 4), cols = random_int(0, 4);
        std::vector<std::string> grid(rows, std::string(cols, '0'));
        Matrix matrix(rows, std::vector<i64>(cols));
        for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c) {
            grid[r][c] = static_cast<char>('0' + random_int(0, 1));
            matrix[r][c] = random_int(0, 6);
        }
        i64 rectangle = 0;
        for (int top = 0; top < rows; ++top) for (int bottom = top; bottom < rows; ++bottom)
            for (int left = 0; left < cols; ++left) for (int right = left; right < cols; ++right) {
                bool valid = true;
                for (int r = top; r <= bottom; ++r) for (int c = left; c <= right; ++c)
                    if (grid[r][c] != '1') valid = false;
                if (valid) rectangle = std::max(rectangle, 1LL * (bottom - top + 1) * (right - left + 1));
            }
        check(maximal_rectangle(grid) == rectangle, "matrix rectangles exhaustive");
        constexpr int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
        auto path = [&](auto&& self, int r, int c) -> int {
            int length = 1;
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && matrix[nr][nc] > matrix[r][c])
                    length = std::max(length, 1 + self(self, nr, nc));
            }
            return length;
        };
        int increasing = 0;
        for (int r = 0; r < rows; ++r) for (int c = 0; c < cols; ++c)
            increasing = std::max(increasing, path(path, r, c));
        check(longest_increasing_path(matrix) == increasing, "matrix DAG vs exhaustive paths");
    }
}

void test_design() {
    static_assert(!std::is_copy_constructible<LRUCache>::value, "LRU must not copy foreign iterators");
    MinStack stack;
    check(!stack.pop() && !stack.top() && !stack.minimum(), "empty min stack");
    std::vector<i64> reference;
    for (int step = 0; step < 300; ++step) {
        if (reference.empty() || random_int(0, 1)) {
            i64 x = random_int(-8, 8); stack.push(x); reference.push_back(x);
        } else { check(stack.pop() == reference.back(), "stack pop"); reference.pop_back(); }
        check(stack.top() == (reference.empty() ? std::nullopt : std::optional<i64>{reference.back()}), "stack top");
        check(stack.minimum() == (reference.empty() ? std::nullopt
            : std::optional<i64>{*std::min_element(reference.begin(), reference.end())}), "stack min");
    }
    for (int capacity = 0; capacity <= 6; ++capacity) {
        LRUCache cache(capacity);
        std::vector<std::pair<int, i64>> recent;
        for (int step = 0; step < 250; ++step) {
            int key = random_int(0, 9); i64 value = random_int(-20, 20);
            auto it = std::find_if(recent.begin(), recent.end(), [&](auto entry) { return entry.first == key; });
            if (random_int(0, 1)) {
                cache.put(key, value);
                if (it != recent.end()) recent.erase(it);
                if (capacity) recent.insert(recent.begin(), {key, value});
                if (static_cast<int>(recent.size()) > capacity) recent.pop_back();
            } else {
                std::optional<i64> expected;
                if (it != recent.end()) {
                    auto entry = *it; expected = entry.second;
                    recent.erase(it); recent.insert(recent.begin(), entry);
                }
                check(cache.get(key) == expected, "LRU model differential");
            }
        }
    }
    RandomizedSet values;
    std::set<i64> model;
    check(!values.sample(rng), "empty randomized set");
    for (int step = 0; step < 500; ++step) {
        i64 x = random_int(-10, 10);
        if (random_int(0, 1)) check(values.insert(x) == model.insert(x).second, "random set insertion");
        else check(values.erase(x) == (model.erase(x) != 0), "random set erase");
        check(values.size() == model.size(), "random set size");
        auto sample = values.sample(rng);
        check(model.empty() ? !sample : (sample && model.count(*sample)), "random set sample membership");
    }
}

i64 brute_stock(const std::vector<i64>& prices, int k, i64 fee, bool with_cooldown) {
    auto search = [&](auto&& self, int day, bool hold, int sold, bool cooldown) -> i64 {
        if (day == static_cast<int>(prices.size())) return hold ? -INF : 0;
        i64 best = self(self, day + 1, hold, sold, false);
        if (hold) best = std::max(best, prices[day] - fee + self(self, day + 1, false, sold + 1, with_cooldown));
        else if (!cooldown && sold < k) {
            i64 rest = self(self, day + 1, true, sold, false);
            if (rest != -INF) best = std::max(best, rest - prices[day]);
        }
        return best;
    };
    return search(search, 0, false, 0, false);
}

void test_dp() {
    check(stock_profit_cooldown({1, 2, 3, 0, 2}) == 3, "cooldown example");
    check(burst_balloons({3, 1, 5, 8}) == 167, "balloons example");
    check(word_break("", {}) && !word_break("a", {""}), "word break empty cases");
    for (int rep = 0; rep < 100; ++rep) {
        int n = random_int(0, 9);
        std::vector<i64> a(n);
        for (auto& x : a) x = random_int(-3, 7);
        i64 linear = 0, circular = 0;
        for (int mask = 0; mask < (1 << n); ++mask) if (!(mask & (mask << 1))) {
            i64 sum = 0;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) sum += a[i];
            linear = std::max(linear, sum);
            if (n <= 1 || !((mask & 1) && (mask & (1 << (n - 1))))) circular = std::max(circular, sum);
        }
        check(nonadjacent_sum(a) == linear && nonadjacent_sum(a, true) == circular, "robber exhaustive");
        auto minimax = [&](auto&& self, int l, int r, bool first) -> i64 {
            if (l == r) return 0;
            i64 x = (first ? a[l] : -a[l]) + self(self, l + 1, r, !first);
            i64 y = (first ? a[r - 1] : -a[r - 1]) + self(self, l, r - 1, !first);
            return first ? std::max(x, y) : std::min(x, y);
        };
        check(take_ends_score_difference(a) == minimax(minimax, 0, n, true), "take ends minimax");
        for (auto& x : a) x = random_int(0, 8);
        int k = random_int(0, 5), fee = random_int(0, 3);
        check(stock_profit_k(a, k, fee) == brute_stock(a, k, fee, false), "stock bounded/fee exhaustive");
        check(stock_profit_cooldown(a, fee) == brute_stock(a, n, fee, true), "stock cooldown exhaustive");
        std::string s = text(n), target = text(random_int(0, 5));
        auto pal = palindrome_table(s);
        int min_cuts = n ? n - 1 : 0, partition_count = 0;
        if (!n) partition_count = 1;
        else for (int mask = 0; mask < (1 << (n - 1)); ++mask) {
            int start = 0, pieces = 0; bool valid = true;
            for (int i = 0; i < n; ++i) if (i == n - 1 || (mask & (1 << i))) {
                valid = valid && palindrome(s.substr(start, i - start + 1)); start = i + 1; ++pieces;
            }
            if (valid) { ++partition_count; min_cuts = std::min(min_cuts, pieces - 1); }
        }
        for (int l = 0; l < n; ++l) for (int r = l; r < n; ++r)
            check(static_cast<bool>(pal[l][r]) == palindrome(s.substr(l, r - l + 1)), "palindrome table");
        check(palindrome_min_cuts(s) == min_cuts, "palindrome cuts exhaustive");
        int emitted = 0;
        enumerate_palindrome_partitions(s, [&](const std::vector<std::string>& path) {
            ++emitted; std::string joined;
            for (const auto& piece : path) { check(palindrome(piece), "partition piece"); joined += piece; }
            check(joined == s, "partition concatenation");
        });
        check(emitted == partition_count, "partition output count");
        int subsequences = 0, lcs = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            std::string sub;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) sub += s[i];
            subsequences += sub == target;
            int position = 0;
            for (char ch : target) if (position < static_cast<int>(sub.size()) && sub[position] == ch) ++position;
            if (position == static_cast<int>(sub.size())) lcs = std::max(lcs, position);
        }
        check(distinct_subsequences(s, target, 31) == subsequences % 31, "subsequence count exhaustive");
        check(lcs_length(s, target) == lcs, "LCS compressed row differential");
        std::vector<std::vector<int>> edit(n + 1, std::vector<int>(target.size() + 1));
        for (int i = 0; i <= n; ++i) for (int j = 0; j <= static_cast<int>(target.size()); ++j) {
            if (!i) edit[i][j] = j;
            else if (!j) edit[i][j] = i;
            else edit[i][j] = std::min({edit[i - 1][j] + 1, edit[i][j - 1] + 1,
                edit[i - 1][j - 1] + (s[i - 1] != target[j - 1])});
        }
        check(edit_distance(s, target) == edit[n].back(), "edit compressed row differential");
        std::vector<std::string> dictionary;
        for (int i = 0, size = random_int(0, 7); i < size; ++i) dictionary.push_back(text(random_int(0, 3)));
        std::vector<char> reachable(n + 1);
        reachable[0] = true;
        for (int end = 1; end <= n; ++end) for (const auto& word : dictionary)
            if (!word.empty() && static_cast<int>(word.size()) <= end && s.substr(end - word.size(), word.size()) == word
                && reachable[end - word.size()]) reachable[end] = true;
        check(word_break(s, dictionary) == static_cast<bool>(reachable[n]), "trie word break differential");
        std::string pattern = text(random_int(0, 5));
        for (char& ch : pattern) { int kind = random_int(0, 3); if (!kind) ch = '*'; if (kind == 1) ch = '?'; }
        auto wild = [&](auto&& self, int i, int j) -> bool {
            if (j == static_cast<int>(pattern.size())) return i == n;
            if (pattern[j] == '*') return self(self, i, j + 1) || (i < n && self(self, i + 1, j));
            return i < n && (pattern[j] == '?' || pattern[j] == s[i]) && self(self, i + 1, j + 1);
        };
        check(wildcard_match(s, pattern) == wild(wild, 0, 0), "wildcard exhaustive");
    }
    for (int rep = 0; rep < 20; ++rep) {
        std::vector<i64> a(random_int(0, 7));
        for (auto& x : a) x = random_int(0, 5);
        auto burst = [&](auto&& self, std::vector<i64> remaining) -> i64 {
            i64 best = 0;
            for (int i = 0; i < static_cast<int>(remaining.size()); ++i) {
                i64 value = remaining[i] * (i ? remaining[i - 1] : 1)
                    * (i + 1 < static_cast<int>(remaining.size()) ? remaining[i + 1] : 1);
                auto next = remaining; next.erase(next.begin() + i);
                best = std::max(best, value + self(self, next));
            }
            return best;
        };
        check(burst_balloons(a) == burst(burst, a), "balloons exhaustive removal order");
    }
    for (int bound = 0; bound <= 1500; bound += 37) {
        int count = 0;
        for (int x = 1; x <= bound; ++x) {
            auto s = std::to_string(x);
            count += std::set<char>(s.begin(), s.end()).size() == s.size();
        }
        check(count_unique_digit_numbers(bound) == count, "digit mask DP exhaustive");
    }
    i64 total = 0, ways = 9;
    for (int digits = 1; digits <= 10; ++digits) { total += ways; ways *= 10 - digits; }
    check(count_unique_digit_numbers(1'000'000'000'000'000'000LL) == total, "digit DP full bound");
}

bool brute_word(const std::vector<std::string>& grid, const std::string& word) {
    if (word.empty()) return true;
    int rows = static_cast<int>(grid.size()), cols = rows ? static_cast<int>(grid[0].size()) : 0;
    auto search = [&](auto&& self, int cell, int position, int used) -> bool {
        if ((used & (1 << cell)) || grid[cell / cols][cell % cols] != word[position]) return false;
        if (position + 1 == static_cast<int>(word.size())) return true;
        used |= 1 << cell;
        for (int next = 0; next < rows * cols; ++next)
            if (std::abs(cell / cols - next / cols) + std::abs(cell % cols - next % cols) == 1
                && self(self, next, position + 1, used)) return true;
        return false;
    };
    for (int start = 0; start < rows * cols; ++start) if (search(search, start, 0, 0)) return true;
    return false;
}

void test_search_and_state_graphs() {
    constexpr int catalan[] = {1, 1, 2, 5, 14, 42, 132};
    for (int n = 0; n <= 6; ++n) {
        std::set<std::string> unique;
        enumerate_parentheses(n, [&](const std::string& s) {
            int balance = 0;
            for (char ch : s) { balance += ch == '(' ? 1 : -1; check(balance >= 0, "parenthesis prefix"); }
            check(balance == 0 && static_cast<int>(s.size()) == 2 * n, "balanced parentheses");
            check(unique.insert(s).second, "parenthesis uniqueness");
        });
        check(static_cast<int>(unique.size()) == catalan[n], "Catalan output count");
    }
    constexpr int queens[] = {1, 1, 0, 0, 2, 10, 4, 40, 92};
    for (int n = 0; n <= 8; ++n) {
        int count = 0;
        enumerate_n_queens(n, [&](const std::vector<int>& columns) {
            ++count; check(static_cast<int>(columns.size()) == n, "queen placement size");
            for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
                check(columns[i] != columns[j] && std::abs(columns[i] - columns[j]) != j - i, "queen constraints");
        });
        check(count == queens[n], "N-Queens known counts");
    }
    for (int rep = 0; rep < 80; ++rep) {
        std::vector<i64> candidates(random_int(0, 7));
        for (auto& x : candidates) x = random_int(1, 5);
        int target = random_int(0, 12);
        std::set<std::vector<i64>> once, unlimited;
        int n = static_cast<int>(candidates.size());
        for (int mask = 0; mask < (1 << n); ++mask) {
            std::vector<i64> path;
            for (int i = 0; i < n; ++i) if (mask & (1 << i)) path.push_back(candidates[i]);
            if (std::accumulate(path.begin(), path.end(), 0LL) == target) {
                std::sort(path.begin(), path.end()); once.insert(path);
            }
        }
        auto unique = candidates;
        std::sort(unique.begin(), unique.end()); unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
        std::vector<i64> path;
        auto enumerate = [&](auto&& self, int i, i64 remaining) -> void {
            if (i == static_cast<int>(unique.size())) { if (!remaining) unlimited.insert(path); return; }
            auto size = path.size();
            for (i64 count = 0; count * unique[i] <= remaining; ++count) {
                self(self, i + 1, remaining - count * unique[i]); path.push_back(unique[i]);
            }
            path.resize(size);
        };
        enumerate(enumerate, 0, target);
        auto a = combination_sum(candidates, target, false), b = combination_sum(candidates, target);
        check(std::set<std::vector<i64>>(a.begin(), a.end()) == once && a.size() == once.size(), "one-use combinations");
        check(std::set<std::vector<i64>>(b.begin(), b.end()) == unlimited && b.size() == unlimited.size(), "unlimited combinations");
        int rows = random_int(0, 3), cols = random_int(0, 3);
        std::vector<std::string> grid;
        for (int r = 0; r < rows; ++r) grid.push_back(text(cols));
        auto original = grid;
        std::string word = text(random_int(0, 5));
        check(word_exists(grid, word) == brute_word(grid, word) && grid == original, "grid word search exhaustive");
        std::vector<std::string> dictionary;
        std::set<std::string> expected;
        for (int i = 0; i < 8; ++i) {
            dictionary.push_back(text(random_int(1, 4)));
            if (brute_word(grid, dictionary.back())) expected.insert(dictionary.back());
        }
        dictionary.push_back(dictionary[0]);
        auto found = find_words(grid, dictionary);
        check(std::set<std::string>(found.begin(), found.end()) == expected && found.size() == expected.size(),
            "multiword trie search and duplicates");
        check(grid == original, "multiword search input preserved");
    }
    check(shortest_visit_all({{1, 2, 3}, {0}, {0}, {0}}) == 4, "visit-all requires revisits");
    check(shortest_visit_all({{}, {}}) == -1 && shortest_visit_all({}) == 0, "visit-all empty/disconnected");
    check(shortest_visit_all({{1}, {2}, {}}) == 2 && shortest_visit_all({{1, 2}, {}, {}}) == -1,
        "visit-all directed transitions");
    check(find_words({}, {}).empty() && find_words({"abc"}, {}).empty(), "empty word dictionary");
    for (int rep = 0; rep < 25; ++rep) {
        int n = random_int(1, 7);
        Graph g(n);
        Matrix distance(n, std::vector<i64>(n, INF));
        for (int i = 0; i < n; ++i) distance[i][i] = 0;
        for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) if (random_int(0, 1)) {
            g[i].push_back(j); g[j].push_back(i); distance[i][j] = distance[j][i] = 1;
        }
        distance = floyd_warshall(distance);
        std::vector<int> order(n);
        std::iota(order.begin(), order.end(), 0);
        i64 best = INF;
        do {
            i64 length = 0; bool valid = true;
            for (int i = 1; i < n; ++i) {
                i64 edge = distance[order[i - 1]][order[i]];
                if (edge == INF) { valid = false; break; }
                length += edge;
            }
            if (valid) best = std::min(best, length);
        } while (std::next_permutation(order.begin(), order.end()));
        check(shortest_visit_all(g) == (best == INF ? -1 : best), "state BFS vs metric permutations");
    }
}

std::vector<int> list_values(ListNode* head, int limit) {
    std::vector<int> result;
    while (head) {
        check(static_cast<int>(result.size()) < limit, "no list cycle/extra nodes");
        result.push_back(head->val); head = head->next;
    }
    return result;
}

void test_nodes() {
    for (int rep = 0; rep < 100; ++rep) {
        int n = random_int(0, 24);
        std::vector<ListNode> nodes;
        nodes.reserve(n);
        std::vector<int> expected;
        for (int i = 0; i < n; ++i) { int x = random_int(-3, 4); nodes.emplace_back(x); expected.push_back(x); }
        for (int i = 1; i < n; ++i) nodes[i - 1].next = &nodes[i];
        auto* head = n ? &nodes[0] : nullptr;
        check(middle_node(head) == (n ? &nodes[n / 2] : nullptr), "list second middle");
        std::vector<ListNode*> links;
        for (const auto& node : nodes) links.push_back(node.next);
        auto reversed = expected; std::reverse(reversed.begin(), reversed.end());
        check(palindrome_list(head) == (expected == reversed), "list palindrome");
        for (int i = 0; i < n; ++i) check(nodes[i].next == links[i], "palindrome restored every link");
        int k = random_int(1, n + 3);
        for (int i = 0; i + k <= n; i += k) std::reverse(expected.begin() + i, expected.begin() + i + k);
        head = reverse_k_group(head, k);
        check(list_values(head, n + 1) == expected, "reverse full k-groups");
        head = sort_list(head); std::sort(expected.begin(), expected.end());
        check(list_values(head, n + 1) == expected, "bottom-up list mergesort");
    }
    ListNode a(1), b(2), c(3), d(4);
    a.next = &c; b.next = &d; d.next = &c;
    check(list_intersection(&a, &b) == &c && !list_intersection(&a, nullptr), "shared list tail");
    check(!binary_tree_stats(nullptr).max_path_sum, "empty tree statistics");
    for (int rep = 0; rep < 55; ++rep) {
        int n = random_int(1, 18);
        std::vector<TreeNode> nodes;
        nodes.reserve(n);
        for (int i = 0; i < n; ++i) nodes.emplace_back(random_int(-5, 8));
        std::vector<std::pair<int, bool>> slots{{0, false}, {0, true}};
        std::vector<int> parent(n, -1), depth(n), height(n, 1);
        Graph tree(n);
        for (int i = 1; i < n; ++i) {
            int slot = random_int(0, static_cast<int>(slots.size()) - 1);
            auto [p, right] = slots[slot]; slots[slot] = slots.back(); slots.pop_back();
            if (right) nodes[p].right = &nodes[i]; else nodes[p].left = &nodes[i];
            parent[i] = p; depth[i] = depth[p] + 1;
            slots.emplace_back(i, false); slots.emplace_back(i, true);
            tree[i].push_back(p); tree[p].push_back(i);
        }
        int diameter = 0; i64 max_sum = -INF; bool balanced = true;
        for (int i = n - 1; i >= 0; --i) {
            int left = nodes[i].left ? height[nodes[i].left - nodes.data()] : 0;
            int right = nodes[i].right ? height[nodes[i].right - nodes.data()] : 0;
            height[i] = 1 + std::max(left, right);
            balanced = balanced && std::abs(left - right) <= 1;
        }
        for (int from = 0; from < n; ++from) {
            auto traversal = bfs(tree, {from});
            for (int to = 0; to < n; ++to) {
                diameter = std::max(diameter, traversal.distance[to]);
                i64 sum = 0;
                for (int v = to; v != -1; v = traversal.parent[v]) sum += nodes[v].val;
                max_sum = std::max(max_sum, sum);
                int x = from, y = to;
                while (depth[x] > depth[y]) x = parent[x];
                while (depth[y] > depth[x]) y = parent[y];
                while (x != y) { x = parent[x]; y = parent[y]; }
                check(binary_tree_lca(&nodes[0], &nodes[from], &nodes[to]) == &nodes[x], "pointer LCA differential");
            }
        }
        auto stats = binary_tree_stats(&nodes[0]);
        check(stats.height == height[0] && stats.diameter == diameter && stats.balanced == balanced
            && stats.max_path_sum == max_sum, "postorder tree aggregates");
        i64 total = 0;
        for (const auto& node : nodes) total += node.val;
        check(binary_tree_fold(&nodes[0], 0LL, [](TreeNode* node, i64 left, i64 right) {
            return node->val + left + right;
        }) == total, "generic postorder fold");
        TreeNode absent(0);
        check(!binary_tree_lca(&nodes[0], &nodes[0], &absent), "LCA absent node");
        int target = random_int(-7, 12);
        i64 count = 0;
        for (int end = 0; end < n; ++end) {
            i64 sum = 0;
            for (int v = end; v != -1; v = parent[v]) { sum += nodes[v].val; count += sum == target; }
        }
        check(count_tree_path_sum(&nodes[0], target) == count, "tree prefixes undo between siblings");
    }
    const int n = 100'000;
    std::vector<TreeNode> chain;
    chain.reserve(n);
    for (int i = 0; i < n; ++i) chain.emplace_back(0);
    for (int i = 1; i < n; ++i) chain[i - 1].right = &chain[i];
    auto stats = binary_tree_stats(&chain[0]);
    check(stats.height == n && stats.diameter == n - 1 && !stats.balanced && stats.max_path_sum == 0, "deep binary postorder");
    check(count_tree_path_sum(&chain[0], 0) == 1LL * n * (n + 1) / 2, "deep prefix tree with 64-bit count");
    check(binary_tree_lca(&chain[0], &chain[n / 2], &chain[n - 1]) == &chain[n / 2], "deep pointer LCA");
}
} // namespace

int main() {
    try {
        test_arrays();
        test_windows_and_grids();
        test_design();
        test_dp();
        test_search_and_state_graphs();
        test_nodes();
        std::cout << "Passed " << checks << " LeetCode-pattern checks (seed 20260921).\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAILED after " << checks << " LC checks: " << error.what() << '\n';
        return 1;
    }
}
