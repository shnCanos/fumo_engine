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
    // solve the collision
    Collision collision =
        PlayerToRectCollision(player_shape, player_body, rectangle, rectangle_body);
    // FIXME: (latest) player is colliding completely off the rectangle.
    // check if the issue is with creating the rectangle or if it is with
    // the actual collision code.
    // (we might be updating some values wrong in many places when making rects)

    if (collision.overlap == 0.0f) {
        // no collision happened
        return;
    }

    // -------------------------------------------------------------------------------
    // points towards the player_body
    Vector2 push = Vector2Normalize(collision.push);
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = Vector2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_shape.update_capsule_positions(player_body);
}

void PlayerCollisionRunner::player_to_circle_collision_solving(
    PlayerShape& player_shape, Body& player_body, const Circle& circle_shape,
    const Body& circle_body) {

    // -------------------------------------------------------------------------------
    // solve the collision
    Collision collision =
        PlayerToCircleCollision(player_shape, player_body, circle_shape, circle_body);
    if (collision.overlap == 0.0f) {
        // no collision happened
        return;
    }
    // -------------------------------------------------------------------------------
    // points towards the player_body
    Vector2 push = Vector2Normalize(collision.push);
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = Vector2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_shape.update_capsule_positions(player_body);
}
