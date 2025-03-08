#ifndef POINT_LINE_COLLISIONS_HPP
#define POINT_LINE_COLLISIONS_HPP
#include "raylib.h"
#include <utility>
#include <vector>
[[nodiscard]] float PointToLineDistance(Vector2& Point, Vector2& LineStart,
                                        Vector2& LineEnd);
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(Vector2& Point, Vector2& LineStart, Vector2& LineEnd);
[[nodiscard]] std::pair<float, Vector2>
closest_line(const std::vector<std::pair<float, Vector2>>& distances);
[[nodiscard]] Vector2 closest_point(Vector2 target, const std::vector<Vector2>& points);
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(const Vector2& Point,
                                   const std::pair<Vector2, Vector2>& line);
[[nodiscard]] Vector2 LineToLineIntersection(const std::pair<Vector2, Vector2>& line1,
                                             const std::pair<Vector2, Vector2>& line2);
#endif
