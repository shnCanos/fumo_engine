#include "player_collisions.hpp"
#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/components.hpp"
#include "raymath.h"

// TODO: make player check for collisions with other circles and then with other
// rectangles

extern std::unique_ptr<GlobalState> global;

void PlayerCollisionRunner::check_collisions() {

    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);
    auto& player_body = global->ECS->get_component<Body>(global->player_id);

    EntityQuery query_rectangle{.component_mask =
                                    global->ECS->make_component_mask<Rectangle>(),
                                .component_filter = Filter::All};

    for (const auto& obstacle_id : sys_entities) {
        const auto& obstacle_body = global->ECS->get_component<Body>(obstacle_id);

        if (global->ECS->filter(obstacle_id, query_rectangle)) {
            const auto& rectangle = global->ECS->get_component<Rectangle>(obstacle_id);
            player_to_rect_collision_solving(player_shape, player_body, rectangle,
                                             obstacle_body);
        } else {
            const auto& circle_shape = global->ECS->get_component<Circle>(obstacle_id);
            player_to_circle_collision_solving(player_shape, player_body, circle_shape,
                                               obstacle_body);
        }
    }
}

void PlayerCollisionRunner::player_to_rect_collision_solving(
    PlayerShape& player_shape, Body& player_body, const Rectangle& rectangle,
    const Body& rectangle_body) {

    // NOTE: assume rectangles are NOT rotated
    // (add rotation support later if i want to add slopes and stuff)

    // -------------------------------------------------------------------------------
    // Explanation:
    // -> Checking agaisnt the capsule's circles
    // 1- we take our circle and find the distance between the center of the circle
    //  and the closest point to the circle's center,
    //  on any of the sides of the rectangle.
    //  then we take the closest side and pick that as our collision side
    //  and move the circle away from that point of intersection
    //
    // 2- if neither side is valid that means we are colliding with the corner.
    //  this means we want to move the circle away from the corner, so we take
    //  the closest corner and solve for that
    //
    // -> Checking agaisnt the capsule's sides:
    // - this works similar to RectangleToRectangle collisions, but we
    // only need to check if either of the capsule's sides are inside the rectangle
    // if they are, we move away
    // -------------------------------------------------------------------------------

    // FIXME:
    // -------------------------------------------------------------------------------
    // change to this checking agaisnt the CAPSULE shape
    // -------------------------------------------------------------------------------

    Rectangle rect;
    Circle circle;
    Vector2 circle_center;

    Vector2 TopLeft = {rect.x, rect.y};
    Vector2 TopRight = {rect.x + rect.width, rect.y};
    Vector2 BottomLeft = {rect.x, rect.y + rect.height};
    Vector2 BottomRight = {rect.x + rect.width, rect.y + rect.height};

    std::pair<float, Vector2> distance_to_left, distance_to_bottom, distance_to_right,
        distance_to_top;

    distance_to_left =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
    distance_to_top =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
    distance_to_right =
        PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
    distance_to_bottom =
        PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);

    std::vector<std::pair<float, Vector2>> distances = {
        distance_to_left, distance_to_top, distance_to_right, distance_to_bottom};

    std::pair<float, Vector2> closest_pair = closest_line(distances);

    // -------------------------------------------------------------------------------
    // solve overlapping
    // -------------------------------------------------------------------------------
    float intersection_distance = closest_pair.first;
    Vector2 closest_intersection = closest_pair.second;

    // -------------------------------------------------------------------------------
    // if no side a direct collision, then
    // find the closest corner and move away from it
    if (intersection_distance == 0) {
        std::vector<Vector2> points{TopLeft, TopRight, BottomLeft, BottomRight};
        closest_intersection = closest_point(circle_center, points);
        intersection_distance = Vector2Distance(circle_center, closest_intersection);
    }
    // -------------------------------------------------------------------------------

    float overlap = circle.radius - intersection_distance;
    float correction = 1.0f; // NOTE: this value is here to stabilize the collision

    Vector2 push_direction = circle_center - closest_intersection;
    Vector2 push = Vector2Normalize(push_direction);

    push = Vector2Scale(push, overlap * correction);
    circle_center += push;
}

void PlayerCollisionRunner::player_to_circle_collision_solving(
    PlayerShape& player_shape, Body& player_body, const Circle& circle_shape,
    const Body& circle_body) {
    // -------------------------------------------------------------------------------
    // check for collisions with capsule
    // -------------------------------------------------------------------------------

    // -------------------------------------------------------------------------------
    // capsule sides collision check
    const auto left_line_distance_pair =
        PointToLineDistanceAndIntersection(circle_body.position, player_shape.left_line);
    const auto right_line_distance_pair = PointToLineDistanceAndIntersection(
        circle_body.position, player_shape.right_line);
    // -------------------------------------------------------------------------------
    // top circle collision check
    float top_distance =
        Vector2Distance(circle_body.position, player_shape.top_circle_center);
    // -------------------------------------------------------------------------------
    // bottom circle collision check
    float bottom_distance =
        Vector2Distance(circle_body.position, player_shape.bottom_circle_center);
    // -------------------------------------------------------------------------------

    // -------------------------------------------------------------------------------
    // go through collision order
    float radius_sum = player_shape.radius + circle_shape.radius;
    float overlap = -6969;
    Vector2 push_direction;

    if (left_line_distance_pair.first != 0 &&
        left_line_distance_pair.first < circle_shape.radius) {
        // collided with left_line
        overlap = circle_shape.radius - left_line_distance_pair.first;
        push_direction = left_line_distance_pair.second - circle_body.position;
    }
    if (right_line_distance_pair.first != 0 &&
        right_line_distance_pair.first < circle_shape.radius) {
        // collided with right_line
        overlap = circle_shape.radius - right_line_distance_pair.first;
        push_direction = right_line_distance_pair.second - circle_body.position;
    }
    if (top_distance < radius_sum) {
        // collided with top circle
        overlap = radius_sum - top_distance;
        push_direction = player_shape.top_circle_center - circle_body.position;
    }
    if (bottom_distance < radius_sum) {
        // collided with top circle
        overlap = radius_sum - bottom_distance;
        push_direction = player_shape.bottom_circle_center - circle_body.position;
    }
    if (overlap == -6969) {
        return;
    }

    // -------------------------------------------------------------------------------
    // solve the collision
    // -------------------------------------------------------------------------------

    // points towards the player_body
    Vector2 push = Vector2Normalize(push_direction);
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = Vector2Scale(push, overlap * correction);
    player_body.position += push;
    player_shape.update_capsule_positions(player_body);
}
