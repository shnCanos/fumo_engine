
#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP
#include "objects/components.hpp"
#include "raylib.h"
Vector2 Vector2ScaleDivision(Vector2 v, float scale);
void debug_print_animation_info(const AnimationInfo& animation_info);
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
#endif
