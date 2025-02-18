#include "player_physics.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "raymath.h"
#include <unistd.h>

extern std::unique_ptr<GlobalState> global;

void PlayerPhysicsRunner::run_physics() {

    auto& player_body = global->ECS.get_component<Body>(global->player_id);
    auto& player_shape = global->ECS.get_component<CircleShape>(global->player_id);
    update_gravity(player_body, player_shape);
}

void PlayerPhysicsRunner::update_gravity(Body& player_body, CircleShape& player_shape) {
    auto circle_physics_ptr = global->ECS.get_system<CirclePhysicsHandler>();
    // auto rect_physics_ptr = global->ECS.get_system<RectanglePhysicsHandler>();

    circle_physics_ptr->find_gravity_field(player_body, player_shape);
}

void PlayerPhysicsRunner::update_position(Body& player_body) {
    player_body.position += player_body.velocity * global->frametime;
}

void CirclePhysicsHandler::find_gravity_field(Body& entity_body,
                                              CircleShape& entity_shape) {
    // NOTE: we should only let one planet affect the player at each time
    // solution: check for being inside a planet's gravity field range
    // if we are, then we get pulled by that, but if we get affected by another one
    // after, we change to that one instead, only allowing one planet entity
    // to affect the player position at a time b
    // 

    for (auto circle_id : sys_entities) {
        // FIXME: check that "Only" filter will remove the player id from the array
        
        // FIXME: create a "gravity radius" variable to define the reach of each planet
        // towards the player
        
        // FIXME: add a buffer that doesnt allow swapping orbits more than once a second too


        auto circle_body = global->ECS.get_component<Body>(circle_id);
        auto circle_shape = global->ECS.get_component<CircleShape>(circle_id);

        float distance = Vector2Distance(circle_body.position, entity_body.position);
        float radius_sum = entity_shape.radius + circle_shape.radius;

        if (distance < radius_sum) {
            update_gravity(circle_body, entity_body);
        }
    }
}

void CirclePhysicsHandler::update_gravity(Body& circle_body, Body& entity_body) {
    // TODO: add the update gravity code here
}

void RectanglePhysicsHandler::update_gravity(Body& entity_body) {
    for (auto rectangle_id : sys_entities) {
        auto rectangle_body = global->ECS.get_component<Body>(rectangle_id);
        auto rect_shape = global->ECS.get_component<RectangleShape>(rectangle_id);
        // TODO: change the Y axis of the body (player) to match
        // the side of the rectangle we are on vertically
    }
}
// for (auto circle_id : sys_entities) {
//     auto circle_body = global->ECS.get_component<Body>(circle_id);
//     auto circle_shape = global->ECS.get_component<CircleShape>(circle_id);
//     // TODO: change the Y axis of the body (player) to match
//     // the center of the circle planet
//
//     Vector2 force_direction =
//         Vector2Normalize(circle_body.position - entity_body.position);
//     // F = (G x m1 x m2) / r²; F = m x a
//     // rewritten as:
//     //       a = (G x m2) / r²
//     float sqr_distance =
//         Vector2DistanceSqr(circle_body.position, entity_body.position);
//     Vector2 acceleration =
//         force_direction * GRAVITATIONAL_CONSTANT * circle_body.mass / sqr_distance;
//     // multiplied by direction to turn magnitude into vectorial change
//     entity_body.velocity += acceleration * global->frametime;
// }
