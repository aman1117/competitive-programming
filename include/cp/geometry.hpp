#pragma once
#include "common.hpp"

namespace cp {
struct Point {
    i64 x, y;
    bool operator<(const Point& other) const { return std::tie(x, y) < std::tie(other.x, other.y); }
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};
// Integer geometry: require |coordinate| <= 10^9 so differences/products and
// cross-product subtraction fit signed i64. Larger coordinates need wider math.
// TC/SC O(1). Positive = counterclockwise, negative = clockwise, zero = collinear.
inline i64 cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Closed segment membership, including degenerate point segments. TC/SC O(1).
inline bool on_segment(Point a, Point b, Point p) {
    return cross(a, b, p) == 0 && std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x)
        && std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

// Closed segment intersection, including touching/overlapping. TC/SC O(1).
inline bool segments_intersect(Point a, Point b, Point c, Point d) {
    i64 x = cross(a, b, c), y = cross(a, b, d), z = cross(c, d, a), w = cross(c, d, b);
    if (!x && on_segment(a, b, c)) return true;
    if (!y && on_segment(a, b, d)) return true;
    if (!z && on_segment(c, d, a)) return true;
    if (!w && on_segment(c, d, b)) return true;
    // Compare signs instead of multiplying cross products (which can overflow).
    return ((x < 0 && y > 0) || (x > 0 && y < 0))
        && ((z < 0 && w > 0) || (z > 0 && w < 0));
}

// Andrew monotone chain: CCW hull, no repeated first vertex.
// Discards collinear boundary interior points; all-collinear -> two endpoints.
// TC O(n log n), SC O(n) by-value input + upper/lower chains.
inline std::vector<Point> convex_hull(std::vector<Point> points) {
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1) return points;
    std::vector<Point> lower, upper;
    for (Point p : points) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), p) <= 0) lower.pop_back();
        lower.push_back(p);
    }
    for (auto it = points.rbegin(); it != points.rend(); ++it) {
        while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper.back(), *it) <= 0) upper.pop_back();
        upper.push_back(*it);
    }
    lower.pop_back(); upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

// Shoelace: absolute TWICE area, keeping half-unit areas exact.
// Simple polygon in boundary order. TC O(n), SC O(1).
// All accumulated cross terms must fit i64; coordinate bounds alone do not
// guarantee this for arbitrarily many vertices.
inline i64 twice_polygon_area(const std::vector<Point>& polygon) {
    i64 sum = 0;
    for (int i = 0; i < static_cast<int>(polygon.size()); ++i) {
        Point a = polygon[i], b = polygon[(i + 1) % polygon.size()];
        sum += a.x * b.y - a.y * b.x;
    }
    assert(sum != std::numeric_limits<i64>::min());
    return sum < 0 ? -sum : sum;
}
} // namespace cp
