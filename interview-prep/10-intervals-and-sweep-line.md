# Intervals & Sweep Line: Google / Uber Level

[← All interview topics](README.md) · Handbook: [Monotonic structures and greedy §7 (intervals)](../docs/monotonic-and-greedy/README.md), [Range queries and offline processing](../docs/range-queries/README.md)

⭐ = must do · 🔒 = LeetCode Premium · **Asked at**: `G` = Google tag, `U` = Uber tag, 🔥 = hot right now (frequent on Google's last-3-months or Uber's last-6-months tag). [How these were chosen](README.md#how-problems-were-chosen).

---

## How to practice this topic

If short on time, do only the 10 ⭐ problems first.

**Recognise it when:** meetings, bookings, rides, time ranges, or "how many overlap at once?". Uber phrases many questions this way (rides, drivers, calendars).

1. **Sort by start** to merge; **sort by end** to choose the maximum number of non-overlapping intervals (classic greedy proof: earliest finish leaves the most room).
2. **Sweep line:** turn each interval into `(start, +1)` and `(end, −1)` events, sort, and keep a running count. Decide whether an end at time `t` happens before a start at `t`.
3. **Difference array** when coordinates are small; **ordered map** (`std::map`) when they are large or online.
4. **Heap of end times** for "how many rooms" and "which room" (Meeting Rooms II/III).
5. **Ordered set of disjoint intervals** for online insert/query (My Calendar, Range Module).

---

## 1. Merge & Insert

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 56 | [Merge Intervals](https://leetcode.com/problems/merge-intervals/) ⭐ | Medium | sort by start, extend end | `G` `U` 🔥 |
| ☐ | 57 | [Insert Interval](https://leetcode.com/problems/insert-interval/) ⭐ | Medium | three phases: before, overlap, after | `G` `U` |
| ☐ | 986 | [Interval List Intersections](https://leetcode.com/problems/interval-list-intersections/) | Medium | two pointers, advance smaller end | `G` |
| ☐ | 1288 | [Remove Covered Intervals](https://leetcode.com/problems/remove-covered-intervals/) | Medium | sort by (start, −end) | `G` |
| ☐ | 759 | [Employee Free Time](https://leetcode.com/problems/employee-free-time/) 🔒 | Hard | merge all schedules, find gaps | `G` 🔥 |

## 2. Greedy Selection

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 435 | [Non-overlapping Intervals](https://leetcode.com/problems/non-overlapping-intervals/) ⭐ | Medium | sort by end, count kept | `G` `U` |
| ☐ | 452 | [Minimum Number of Arrows to Burst Balloons](https://leetcode.com/problems/minimum-number-of-arrows-to-burst-balloons/) | Medium | sort by end, shoot at end | `G` |
| ☐ | 1353 | [Maximum Number of Events That Can Be Attended](https://leetcode.com/problems/maximum-number-of-events-that-can-be-attended/) ⭐ | Medium | sort by start, min-heap of ends | `G` `U` |

## 3. Rooms & Overlap Counting

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 252 | [Meeting Rooms](https://leetcode.com/problems/meeting-rooms/) 🔒 | Easy | sort, check neighbours | `G` `U` |
| ☐ | 253 | [Meeting Rooms II](https://leetcode.com/problems/meeting-rooms-ii/) ⭐ 🔒 | Medium | min-heap of ends or sweep | `G` `U` 🔥 |
| ☐ | 2402 | [Meeting Rooms III](https://leetcode.com/problems/meeting-rooms-iii/) ⭐ | Hard | free-room heap + busy heap by (end, room) | `G` `U` 🔥 |
| ☐ | 1094 | [Car Pooling](https://leetcode.com/problems/car-pooling/) ⭐ | Medium | difference array / sweep | `G` |
| ☐ | 1229 | [Meeting Scheduler](https://leetcode.com/problems/meeting-scheduler/) 🔒 | Medium | sort both, two pointers | `G` `U` |
| ☐ | 2251 | [Number of Flowers in Full Bloom](https://leetcode.com/problems/number-of-flowers-in-full-bloom/) | Hard | sorted starts/ends + binary search | `G` `U` |
| ☐ | 1851 | [Minimum Interval to Include Each Query](https://leetcode.com/problems/minimum-interval-to-include-each-query/) | Hard | sort queries, heap by size | `G` |

## 4. Online Intervals (Ordered Map)

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 729 | [My Calendar I](https://leetcode.com/problems/my-calendar-i/) ⭐ | Medium | map: check predecessor and successor | `G` `U` |
| ☐ | 731 | [My Calendar II](https://leetcode.com/problems/my-calendar-ii/) | Medium | sweep counts ≤ 2 | `G` |
| ☐ | 732 | [My Calendar III](https://leetcode.com/problems/my-calendar-iii/) | Hard | sweep map, max running count | `G` |
| ☐ | 715 | [Range Module](https://leetcode.com/problems/range-module/) ⭐ | Hard | map of disjoint intervals | `G` |
| ☐ | 352 | [Data Stream as Disjoint Intervals](https://leetcode.com/problems/data-stream-as-disjoint-intervals/) | Hard | map, merge with neighbours | `G` |
| ☐ | 2276 | [Count Integers in Intervals](https://leetcode.com/problems/count-integers-in-intervals/) | Hard | map of disjoint intervals + total | `G` `U` |

## 5. Sweep Line with a Heap

| Done | # | Problem | Level | Pattern | Asked at |
|---|---|---|---|---|---|
| ☐ | 218 | [The Skyline Problem](https://leetcode.com/problems/the-skyline-problem/) ⭐ | Hard | events + max-heap / multiset of heights | `G` `U` |

---

**Interview-ready when:** you can explain the earliest-end-first exchange argument and handle touching endpoints (`[1,2]` and `[2,3]`) correctly without being prompted.
