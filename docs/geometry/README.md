# Integer geometry

[Handbook](../README.md) | [Implementation](../../include/cp/geometry.hpp)

**Prerequisites:** vectors in two dimensions and signed arithmetic.  
**Goal:** use exact orientation predicates instead of fragile slope comparisons or unnecessary floating point.

## Numeric contract

`Point` uses `i64` coordinates. Require absolute coordinate values at most `10^9` so coordinate differences, products, and cross-product subtraction fit signed 64-bit arithmetic. This bound does not automatically guarantee that a sum over an arbitrarily large polygon fits; accumulated area terms have their own bound.

## 1. Orientation / cross product

**Use when:** deciding whether three points turn left, turn right, or lie on a line.

For points `a,b,c`, compute:

```text
(b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x)
```

Positive means counterclockwise, negative means clockwise, and zero means collinear.

```cpp
assert(cp::cross({0, 0}, {2, 0}, {1, 1}) == 2);
assert(cp::cross({0, 0}, {2, 0}, {1, -1}) == -2);
assert(cp::cross({0, 0}, {2, 0}, {1, 0}) == 0);
```

Moving from `(0,0)` to `(2,0)` points right; `(1,1)` lies to the left of that directed edge.

**TC/SC:** `O(1)` time and space. Comparing slopes introduces division, vertical-line special cases, and rounding. The cross product avoids all three.

## 2. Point-on-segment and segment intersection

**Use when:** closed segments may cross, touch, overlap, or degenerate into a point.

A point is on a segment if it is collinear and lies inside both coordinate bounding intervals. Collinearity alone is not sufficient: `(3,0)` is on the line through `(0,0),(2,0)` but outside the segment.

For non-collinear intersections, each segment's endpoints must lie on opposite sides of the other segment's line. Handle collinear boundary cases separately.

```cpp
assert(cp::on_segment({0, 0}, {2, 0}, {1, 0}));
assert(!cp::on_segment({0, 0}, {2, 0}, {3, 0}));
assert(cp::segments_intersect({0, 0}, {3, 3}, {0, 3}, {3, 0}));
assert(cp::segments_intersect({0, 0}, {2, 0}, {1, 0}, {4, 0}));
assert(!cp::segments_intersect({0, 0}, {1, 0}, {2, 0}, {3, 0}));
```

**TC/SC:** `O(1)` time and space. Compare signs directly rather than multiplying two cross products; those products can overflow even when each cross product fits. The API treats touching endpoints and overlapping segments as intersections.

## 3. Convex hull: Andrew's monotone chain

**Use when:** only the outermost boundary of a point set matters.

Sort and deduplicate points lexicographically. Build a lower hull from left to right and an upper hull from right to left. Whenever the newest three points fail to make a strict left turn, remove the middle point: it cannot belong to that convex boundary chain.

For a square plus center `(1,1)` and edge point `(1,0)`, the center is interior and the edge point is redundant under the chosen collinearity policy.

```cpp
auto hull = cp::convex_hull({{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}, {1, 0}, {0, 0}});
assert((hull == std::vector<cp::Point>{{0, 0}, {2, 0}, {2, 2}, {0, 2}}));
assert(cp::convex_hull({{0, 0}, {1, 0}, {2, 0}}).size() == 2);
```

**TC/SC:** `O(n log n)` time from sorting and `O(n)` space including the copied input and chains. Each point enters and leaves each chain at most once, so hull construction after sorting is linear.

The result is counterclockwise without repeating the first point. Interior collinear boundary points are discarded; all-collinear input returns the two extreme endpoints. If a problem requires every boundary point, its collinearity handling must be adapted rather than assumed.

## 4. Polygon area: shoelace formula

**Use when:** a simple polygon is supplied in boundary order.

Sum signed cross terms for consecutive vertices and the closing edge:

```text
twice_area = abs(sum(x[i]*y[i+1] - y[i]*x[i+1]))
```

For triangle `(0,0),(4,0),(0,3)`, the signed sum is `12`, so the area is six.

```cpp
assert(cp::twice_polygon_area({{0, 0}, {4, 0}, {0, 3}}) == 12);
assert(cp::twice_polygon_area({{0, 0}, {0, 3}, {4, 0}}) == 12);
```

**TC/SC:** `O(n)` time and `O(1)` workspace. Returning twice the area preserves exact half-integer areas without floating point. Reversing orientation changes the sign before the absolute value, not the final area.

Do not pass points in arbitrary order and expect a polygon area. Self-intersections have different signed-area semantics, and accumulated cross terms must stay inside the integer range.
