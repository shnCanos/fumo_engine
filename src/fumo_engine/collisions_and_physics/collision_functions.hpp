#pragma once
#include <utility>
#include <vector>

#include "fumo_engine/components.hpp"
#include "objects/generic_systems/systems.hpp"
enum struct SHAPE { NO_SHAPE, Rectangle, Circle };

// WARNING:
// not all collision information is guaranteed to be filled!
struct Collision {
    float overlap = 0.0f;
    // NOTE: if the collision is with a rectangle corner,
    // it wont be normal to one of the sides,
    // it will connect the collided corner with the shape at some angle
    FumoVec2 normal_or_push = {0.0f, 0.0f};
    FumoVec2 normal = {0.0f, 0.0f};
    DIRECTION capsule_collided_side = DIRECTION::NO_DIRECTION;
    // NOTE: there isnt an intersection point between 2 circles
    FumoVec2 intersection_point = {0.0f, 0.0f};
    float distance = 0.0f;
    bool collided = false;
    SHAPE collided_shape = SHAPE::NO_SHAPE;
};

struct RectBodyPair {
    FumoRect fumo_rect;
    Body body;
};

namespace Collisions {
[[nodiscard]] bool continuous_rect_collision_solving(Capsule& player_shape,
                                                     Body& player_body,
                                                     const FumoRect& fumo_rect,
                                                     const Body& fumo_rect_body,
                                                     const int& substep_count);
[[nodiscard]] std::vector<RectBodyPair>
calculate_sub_rectangles(const FumoRect& fumo_rect,
                         const Body& fumo_rect_body,
                         const int& substep_count,
                         const int& max_sub_distance);

bool capsule_to_rect_collision_solving(Capsule& player_shape,
                                       Body& player_body,
                                       const FumoRect& fumo_rect,
                                       const Body& fumo_rect_body);

bool capsule_to_circle_collision_solving(Capsule& player_shape,
                                         Body& player_body,
                                         const Circle& circle_shape,
                                         const Body& circle_body,
                                         const int& substep_count);
[[nodiscard]] std::pair<float, FumoVec2>
PointToLineDistanceAndIntersection(const FumoVec2& Point,
                                   const FumoVec2& LineStart,
                                   const FumoVec2& LineEnd);
[[nodiscard]] Collision PointToLineCollision(const FumoVec2& Point,
                                             const Line& line);
[[nodiscard]] std::pair<float, FumoVec2> closest_fumo_rect_side(
    const std::vector<std::pair<float, FumoVec2>>& distances);

[[nodiscard]] FumoVec2 closest_point(const FumoVec2 target,
                                     const std::vector<FumoVec2>& points);

[[nodiscard]] bool LineToLineCollided(const Line& line1, const Line& line2);

[[nodiscard]] bool LineToRectCollided(const Line& line,
                                      const FumoRect& rect,
                                      const Body& rect_body);

[[nodiscard]] Collision LineToLineCollision(const Line& line1,
                                            const Line& line2);
[[nodiscard]] const Collision LineToRectCollision(const Line& line,
                                                  const FumoRect& rect,
                                                  const Body& rect_body);
[[nodiscard]] const Collision LineToCircleCollision(const Line& line,
                                                    const Circle& circle_shape,
                                                    const Body& circle_body);

[[nodiscard]] FumoVec2 ClosestPointLineIntersection(const FumoVec2& Point,
                                                    const Line& line,
                                                    const float& radius);

[[nodiscard]] std::pair<float, FumoVec2>
CircleToRectDistanceAndIntersection(const FumoVec2& circle_center,
                                    const float& radius,
                                    const FumoRect& rect,
                                    const Body& rect_body);

[[nodiscard]] Collision CapsuleToRectCollision(const Capsule& player_shape,
                                               const Body& player_body,
                                               const FumoRect& fumo_rect,
                                               const Body& rectange_body);

[[nodiscard]] Collision CapsuleToCircleCollision(const Capsule& player_shape,
                                                 const Body& player_body,
                                                 const Circle& circle_shape,
                                                 const Body& circle_body);
} // namespace Collisions
