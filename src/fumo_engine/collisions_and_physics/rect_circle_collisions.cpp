#include "fumo_engine/collisions_and_physics/collision_functions.hpp"

namespace Collisions {

[[nodiscard]] std::pair<float, FumoVec2> closest_fumo_rect_side(
    const std::vector<std::pair<float, FumoVec2>>& distances) {
    // used for picking one of 2 fumo_rect sides for solving a collision
    // with a circle
    // we pick the closest side to the circle center

    std::pair<float, FumoVec2> min_pair {6969696, {0.0f, 0.0f}};

    for (const auto& pair : distances) {
        const auto& dist = pair.first;
        if (dist < min_pair.first && dist != 0) {
            min_pair = pair;
        }
    }
    if (min_pair.first == 6969696) {
        return {0.0f, {0.0f, 0.0f}};
    }
    return min_pair;
}

[[nodiscard]] std::pair<float, FumoVec2>
CircleToRectDistanceAndIntersection(const FumoVec2& circle_center,
                                    const float& radius,
                                    const FumoRect& rect,
                                    const Body& rect_body) {

    FumoVec2 TopLeft = {rect_body.position.x, rect_body.position.y};
    FumoVec2 TopRight = {rect_body.position.x + rect.width,
                         rect_body.position.y};
    FumoVec2 BottomLeft = {rect_body.position.x,
                           rect_body.position.y + rect.height};
    FumoVec2 BottomRight = {rect_body.position.x + rect.width,
                            rect_body.position.y + rect.height};
    std::vector<FumoVec2> points {TopLeft, TopRight, BottomLeft, BottomRight};

    float intersection_distance;
    FumoVec2 closest_corner;
    std::pair<float, FumoVec2> closest_pair = {0.0f, {0.0f, 0.0f}};

    closest_corner = closest_point(circle_center, points);
    intersection_distance = FumoVec2Distance(circle_center, closest_corner);

    if (intersection_distance < radius) {
        // we have a collision with a corner
        // set our corner so it becomes the collision if no side matches
        closest_pair.first = intersection_distance;
        closest_pair.second = closest_corner;
    }
    // -------------------------------------------------------------------------------
    // we have to check the closest sides for collision if none of the corners
    // were a matching collision
    // -------------------------------------------------------------------------------

    std::pair<float, FumoVec2> left_pair, bottom_pair, right_pair, top_pair;

    bottom_pair = PointToLineDistanceAndIntersection(circle_center,
                                                     BottomLeft,
                                                     BottomRight);
    left_pair =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
    right_pair = PointToLineDistanceAndIntersection(circle_center,
                                                    TopRight,
                                                    BottomRight);
    top_pair =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);

    // -------------------------------------------------------------------------------
    // weird_debug_print(closest_pair, left_pair, bottom_pair, right_pair, top_pair);

    if (closest_corner == TopLeft) {
        left_pair = PointToLineDistanceAndIntersection(circle_center,
                                                       TopLeft,
                                                       BottomLeft);
        top_pair = PointToLineDistanceAndIntersection(circle_center,
                                                      TopLeft,
                                                      TopRight);
        closest_pair = closest_fumo_rect_side({left_pair, top_pair});
        return closest_pair;
    }
    if (closest_corner == TopRight) {
        right_pair = PointToLineDistanceAndIntersection(circle_center,
                                                        TopRight,
                                                        BottomRight);
        top_pair = PointToLineDistanceAndIntersection(circle_center,
                                                      TopLeft,
                                                      TopRight);
        closest_pair = closest_fumo_rect_side({right_pair, top_pair});
        return closest_pair;
    }
    if (closest_corner == BottomLeft) {
        bottom_pair = PointToLineDistanceAndIntersection(circle_center,
                                                         BottomLeft,
                                                         BottomRight);
        left_pair = PointToLineDistanceAndIntersection(circle_center,
                                                       TopLeft,
                                                       BottomLeft);
        closest_pair = closest_fumo_rect_side({bottom_pair, left_pair});
        return closest_pair;
    }
    if (closest_corner == BottomRight) {
        right_pair = PointToLineDistanceAndIntersection(circle_center,
                                                        TopRight,
                                                        BottomRight);
        bottom_pair = PointToLineDistanceAndIntersection(circle_center,
                                                         BottomLeft,
                                                         BottomRight);
        closest_pair = closest_fumo_rect_side({right_pair, bottom_pair});
        return closest_pair;
    }
    return closest_pair;
}

// -------------------------------------------------------------------------------
// Explanation:
// -> Checking agaisnt the capsule's circles
// 1- we take our circle and find the distance between the center of the circle
//  and the closest point to the circle's center,
//  on any of the sides of the fumo_rect.
//  then we take the closest side and pick that as our collision side
//  and move the circle away from that point of intersection
//
// 2- if neither side is valid that means we are colliding with the corner.
//  this means we want to move the circle away from the corner, so we take
//  the closest corner and solve for that
// -------------------------------------------------------------------------------
[[nodiscard]] Collision CapsuleToRectCollision(const Capsule& player_capsule,
                                               const Body& player_body,
                                               const FumoRect& fumo_rect,
                                               const Body& rect_body) {

    std::pair<float, FumoVec2> closest_dist_intersection = {0.0f, {0.0f, 0.0f}};
    Collision collision {};
    // -------------------------------------------------------------------------------
    // try the bottom circle for collisions
    closest_dist_intersection =
        CircleToRectDistanceAndIntersection(player_capsule.bottom_circle_center,
                                            player_capsule.radius,
                                            fumo_rect,
                                            rect_body);

    if (closest_dist_intersection.first != 0.0f) {
        // means any side falls along our center
        if (closest_dist_intersection.first < player_capsule.radius) {
            // PRINT_NO_NAME("COLLISION HAPPENED BOTTOM")
            // this means we collided with the bottom circle
            collision.overlap =
                player_capsule.radius - closest_dist_intersection.first;
            collision.normal_or_push =
                FumoVec2Normalize(player_capsule.bottom_circle_center
                                  - closest_dist_intersection.second);
            collision.capsule_collided_side = DIRECTION::UP;
            collision.collided = true;
            collision.intersection_point = closest_dist_intersection.second;

            if (LineToRectCollided(player_capsule.left_line,
                                   fumo_rect,
                                   rect_body)) {
                collision.capsule_collided_side = DIRECTION::LEFT;
                // PRINT_NO_NAME("COLLISION HAPPENED LEFT")
            }
            if (LineToRectCollided(player_capsule.right_line,
                                   fumo_rect,
                                   rect_body)) {
                collision.capsule_collided_side = DIRECTION::RIGHT;

                // PRINT_NO_NAME("COLLISION HAPPENED RIGHT")
            }
            return collision;
        }
    }
    // -------------------------------------------------------------------------------
    // // try the top circle for collisions
    closest_dist_intersection =
        CircleToRectDistanceAndIntersection(player_capsule.top_circle_center,
                                            player_capsule.radius,
                                            fumo_rect,
                                            rect_body);

    if (closest_dist_intersection.first != 0.0f) {
        // this means we collided with the top circle
        if (closest_dist_intersection.first < player_capsule.radius) {
            // PRINT_NO_NAME("COLLISION HAPPENED TOP")
            collision.overlap =
                player_capsule.radius - closest_dist_intersection.first;
            collision.normal_or_push =
                FumoVec2Normalize(player_capsule.bottom_circle_center
                                  - closest_dist_intersection.second);
            collision.capsule_collided_side = DIRECTION::DOWN;
            collision.intersection_point = closest_dist_intersection.second;
            collision.collided = true;
        }
    }

    if (LineToRectCollided(player_capsule.left_line, fumo_rect, rect_body)) {
        collision.capsule_collided_side = DIRECTION::LEFT;
        // PRINT_NO_NAME("COLLISION HAPPENED LEFT")
    }
    if (LineToRectCollided(player_capsule.right_line, fumo_rect, rect_body)) {
        collision.capsule_collided_side = DIRECTION::RIGHT;

        // PRINT_NO_NAME("COLLISION HAPPENED RIGHT")
    }
    return collision;
}

[[nodiscard]] Collision CapsuleToCircleCollision(const Capsule& player_capsule,
                                                 const Body& player_body,
                                                 const Circle& circle_shape,
                                                 const Body& circle_body) {

    // TODO: this is older code, eventually you should replace all
    // instances of std::pair<float, FumoVec2> (used for dist_intersection)
    // with just filling in fields of the Collision struct and returning it
    // (theres one function that uses this std::pair for other purposes though)

    float radius_sum = player_capsule.radius + circle_shape.radius;
    float overlap = 0.0f;
    FumoVec2 push_direction {0.0f, 0.0f};

    float correction = 0.5f;
    // -------------------------------------------------------------------------------
    // bottom circle collision check
    float bottom_distance =
        FumoVec2Distance(circle_body.position,
                         player_capsule.bottom_circle_center);
    if (bottom_distance < radius_sum + correction) {
        // collided with the bottom circle
        overlap = radius_sum - bottom_distance;
        push_direction = FumoVec2Normalize(player_capsule.bottom_circle_center
                                           - circle_body.position);
        return {.overlap = overlap,
                .normal_or_push = push_direction,
                .distance = bottom_distance,
                .collided = true};
    }
    // -------------------------------------------------------------------------------
    // top circle collision check
    float top_distance = FumoVec2Distance(circle_body.position,
                                          player_capsule.top_circle_center);

    if (top_distance < radius_sum) {
        // collided with top circle
        overlap = radius_sum - top_distance;
        push_direction = FumoVec2Normalize(player_capsule.top_circle_center
                                           - circle_body.position);

        return {.overlap = overlap,
                .normal_or_push = push_direction,
                .distance = top_distance,
                .collided = true};
    }
    // -------------------------------------------------------------------------------
    // NOTE: remove the capsule sides later but for now i want to test them
    //
    // -------------------------------------------------------------------------------
    // capsule sides collision check
    const auto left_line_distance_pair =
        PointToLineDistanceAndIntersection(circle_body.position,
                                           player_capsule.left_line.start,
                                           player_capsule.left_line.end);

    if (left_line_distance_pair.first != 0
        && left_line_distance_pair.first < circle_shape.radius) {
        // collided with left_line
        overlap = circle_shape.radius - left_line_distance_pair.first;
        push_direction = FumoVec2Normalize(left_line_distance_pair.second
                                           - circle_body.position);
        return {.overlap = overlap,
                .normal_or_push = push_direction,
                .intersection_point = left_line_distance_pair.second,
                .distance = left_line_distance_pair.first,
                .collided = true};
    }

    const auto right_line_distance_pair =
        PointToLineDistanceAndIntersection(circle_body.position,
                                           player_capsule.right_line.start,
                                           player_capsule.right_line.end);

    if (right_line_distance_pair.first != 0
        && right_line_distance_pair.first < circle_shape.radius) {
        // collided with right_line
        overlap = circle_shape.radius - right_line_distance_pair.first;
        push_direction = FumoVec2Normalize(right_line_distance_pair.second
                                           - circle_body.position);
        return {.overlap = overlap,
                .normal_or_push = push_direction,
                .intersection_point = right_line_distance_pair.second,
                .distance = right_line_distance_pair.first,
                .collided = true};
    }

    // -------------------------------------------------------------------------------
    // no collision happened
    return {overlap, push_direction};
}

bool capsule_to_rect_collision_solving(Capsule& player_capsule,
                                       Body& player_body,
                                       const FumoRect& fumo_rect,
                                       const Body& fumo_rect_body) {
    // NOTE: assume fumo_rects are NOT rotated
    // (add rotation support later if i want to add slopes and stuff)
    // FumoRect doesnt store the position, that is stored in the Body
    // (this is useful for writing generic move functions)

    // -------------------------------------------------------------------------------
    // solve the collision
    // FIXME: current |
    // add a "sub_step_count"
    Collision collision = Collisions::CapsuleToRectCollision(player_capsule,
                                                             player_body,
                                                             fumo_rect,
                                                             fumo_rect_body);

    if (!collision.collided) {
        // no collision happened
        return false;
    }
    // -------------------------------------------------------------------------------
    // points towards the player_body
    FumoVec2 push = collision.normal_or_push;
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = FumoVec2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_capsule.update_capsule_positions(player_body);
    return true;
}

bool capsule_to_circle_collision_solving(Capsule& player_capsule,
                                         Body& player_body,
                                         const Circle& circle_shape,
                                         const Body& circle_body,
                                         const int& substep_count) {
    // -------------------------------------------------------------------------------
    // solve the collision
    Collision collision = Collisions::CapsuleToCircleCollision(player_capsule,
                                                               player_body,
                                                               circle_shape,
                                                               circle_body);
    if (!collision.collided) {
        // no collision happened
        return false;
    }
    // -------------------------------------------------------------------------------
    // points towards the player_body
    FumoVec2 push = collision.normal_or_push;
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = FumoVec2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_capsule.update_capsule_positions(player_body);
    return true;
}

} // namespace Collisions
