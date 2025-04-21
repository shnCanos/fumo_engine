#pragma once
#include <utility>
#include <vector>

#include "fumo_engine/components.hpp"
#include "raylib.h"

struct Collision {
    float overlap;
    FumoVec2 push;
};

namespace Collisions {
// [[nodiscard]] float PointToLineDistance(const FumoVec2& Point, const FumoVec2&
// LineStart,
//                                         const FumoVec2& LineEnd);
bool player_to_rect_collision_solving(PlayerShape& player_shape,
                                      Body& player_body,
                                      const FumoRect& fumo_rect,
                                      const Body& fumo_rect_body);
bool player_to_circle_collision_solving(PlayerShape& player_shape,
                                        Body& player_body,
                                        const Circle& circle_shape,
                                        const Body& circle_body);
[[nodiscard]] std::pair<float, FumoVec2>
PointToLineDistanceAndIntersection(const FumoVec2& Point,
                                   const FumoVec2& LineStart,
                                   const FumoVec2& LineEnd);
[[nodiscard]] std::pair<float, FumoVec2>
PointToLineDistanceAndIntersection(const FumoVec2& Point,
                                   const std::pair<FumoVec2, FumoVec2>& line);
[[nodiscard]] std::pair<float, FumoVec2> closest_fumo_rect_side(
    const std::vector<std::pair<float, FumoVec2>>& distances);
[[nodiscard]] FumoVec2 closest_point(const FumoVec2 target,
                                     const std::vector<FumoVec2>& points);
// [[nodiscard]] FumoVec2 LineToLineIntersection(const std::pair<FumoVec2, FumoVec2>& line1,
//                                              const std::pair<FumoVec2, FumoVec2>&
//                                              line2);
[[nodiscard]] std::pair<float, FumoVec2>
CircleToRectDistanceAndIntersection(const FumoVec2& circle_center,
                                    const float& radius,
                                    const FumoRect& rect,
                                    const Body& rect_body);

[[nodiscard]] Collision PlayerToRectCollision(const PlayerShape& player_shape,
                                              const Body& player_body,
                                              const FumoRect& fumo_rect,
                                              const Body& rectange_body);

[[nodiscard]] Collision PlayerToCircleCollision(const PlayerShape& player_shape,
                                                const Body& player_body,
                                                const Circle& circle_shape,
                                                const Body& circle_body);
} // namespace Collisions
