#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "player_collisions.hpp"
#include "raymath.h"

// TODO: make player check for collisions with other circles and then with other
// rectangles

extern std::unique_ptr<GlobalState> global;

void PlayerCollisionRunner::check_collisions() {

    auto circle_handler_ptr = global->ECS->get_system<CircleCollisionHandler>();
    // auto rect_handler_ptr = global->ECS->get_system<RectangleCollisionHandler>();

    circle_handler_ptr->check_collisions_with_player();
    // rect_handler_ptr->check_collisions_with_circle(global->player_id);
}

void CircleCollisionHandler::check_collisions_with_player() {
    for (auto circle_entity_id : sys_entities) {
        check_collision_with_player(circle_entity_id, global->player_id);
    }
}

void CircleCollisionHandler::check_collision_with_player(EntityId circle_entity_id,
                                                         EntityId entity_id) {
    // read this to understand how this function works
    // https://dipamsen.github.io/notebook/collisions.pdf

    auto& player_body = global->ECS->get_component<Body>(entity_id);
    auto& circle_body = global->ECS->get_component<Body>(circle_entity_id);

    auto& player_shape = global->ECS->get_component<CircleShape>(entity_id);
    auto& circle_shape = global->ECS->get_component<CircleShape>(circle_entity_id);
    float distance = Vector2Distance(circle_body.position, player_body.position);
    float radius_sum = player_shape.radius + circle_shape.radius;

    double gravity_reach = 600.0f;
    BeginMode2D(*global->camera);
    DrawLineV(player_body.position,
              player_body.position + Vector2Normalize(player_body.velocity) * gravity_reach,
              YELLOW);
    EndMode2D();

    if (distance < radius_sum) {
        solve_collision_player(circle_body, player_body, circle_shape, player_shape);
    }
}
void CircleCollisionHandler::solve_collision_player(Body& other_body, Body& player_body,
                                                    CircleShape& other_shape,
                                                    CircleShape& player_shape) {
    // find new velocity vectors
    float distance = Vector2Distance(other_body.position, player_body.position);
    Vector2 impact = other_body.position - player_body.position;

    // ############################################################################
    // solve overlapping
    float radius_sum = player_shape.radius + other_shape.radius;
    float overlap = radius_sum - distance;

    float correction = 1.0f; // NOTE: this value is here to stabilize the simulation
    Vector2 push = Vector2Normalize(impact);
    // points towards the other body
    // (helpful for finding the new downwards direction for player)
    push = Vector2Scale(push, overlap * correction);
    player_body.position -= push;
    // // ############################################################################
}
