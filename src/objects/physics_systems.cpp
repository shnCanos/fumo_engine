#include "physics_systems.hpp"
#include "../constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "main_functions.hpp"
#include "objects/components.hpp"
#include "raymath.h"
#include <sched.h>

extern std::unique_ptr<GlobalState> global;
void CirclePhysicsUpdate::update_velocity() {

    for (auto entity_id : sys_entities) {
        auto& body = global->ECS.get_component<Body>(entity_id);
        for (auto other_entity_id : sys_entities) {
            if (entity_id == other_entity_id) {
                continue;
            }
            auto& other_body = global->ECS.get_component<Body>(other_entity_id);

            Vector2 force_direction =
                Vector2Normalize(other_body.position - body.position);
            // F = (G x m1 x m2) / r²; F = m x a
            // rewritten as:
            //       a = (G x m2) / r²
            float sqr_distance = Vector2DistanceSqr(other_body.position, body.position);
            Vector2 acceleration =
                force_direction * GRAVITATIONAL_CONSTANT * other_body.mass / sqr_distance;
            // multiplied by direction to turn magnitude into vectorial change
            body.velocity += acceleration * global->frametime;
        }
    }
}
void CirclePhysicsUpdate::detect_screen_collision() {
    for (const auto entity_id : sys_entities) {
        auto& body = global->ECS.get_component<Body>(entity_id);
        auto& shape = global->ECS.get_component<CircleShape>(entity_id);
        float damping = 0.5;
        if (body.position.x + shape.radius > screenWidth) {
            body.velocity.x = -body.velocity.x * damping;
            body.position.x = screenWidth - shape.radius;
        } else if (body.position.x - shape.radius < 0) {
            body.velocity.x = -body.velocity.x * damping;
            body.position.x = shape.radius;
        } else if (body.position.y + shape.radius > screenHeight) {
            body.velocity.y = -body.velocity.y * damping;
            body.position.y = screenHeight - shape.radius;
        } else if (body.position.y - shape.radius < 0) {
            body.velocity.y = -body.velocity.y * damping;
            body.position.y = shape.radius;
        }
    }
}
void CirclePhysicsUpdate::detect_collisions() {

    for (auto entity_id : sys_entities) {
        for (auto other_entity_id : sys_entities) {
            if (entity_id == other_entity_id) {
                continue;
            }
            detect_collision(entity_id, other_entity_id);
        }
    }
}
void CirclePhysicsUpdate::detect_collision(EntityId entity_id, EntityId other_entity_id) {
    // read this to understand how this function works
    // https://dipamsen.github.io/notebook/collisions.pdf

    auto& body = global->ECS.get_component<Body>(entity_id);
    auto& other_body = global->ECS.get_component<Body>(other_entity_id);

    auto& shape = global->ECS.get_component<CircleShape>(entity_id);
    auto& other_shape = global->ECS.get_component<CircleShape>(other_entity_id);
    float distance = Vector2Distance(other_body.position, body.position);
    float radius_sum = shape.radius + other_shape.radius;

    if (distance < radius_sum) {
        solve_collision_aux(body, other_body, shape, other_shape);
    }
}
void CirclePhysicsUpdate::update_position() {

    for (const auto entity_id : sys_entities) {
        auto& body = global->ECS.get_component<Body>(entity_id);
        body.position += body.velocity * global->frametime;
    }
}

void CirclePhysicsUpdate::solve_collision_aux(Body& body, Body& other_body,
                                              CircleShape& shape,
                                              CircleShape& other_shape) {
    // find new velocity vectors
    float distance = Vector2Distance(other_body.position, body.position);

    float mass_sum = body.mass + other_body.mass;

    Vector2 velocity_dif = other_body.velocity - body.velocity;
    Vector2 impact = other_body.position - body.position;

    float relative_change_dot = Vector2DotProduct(velocity_dif, impact);

    Vector2 unit_vector_n = Vector2ScaleDivision(impact, distance * distance * mass_sum);

    // final velocity
    body.velocity +=
        Vector2Scale(unit_vector_n, 2 * other_body.mass * relative_change_dot);
    // other body
    other_body.velocity -=
        Vector2Scale(unit_vector_n, 2 * body.mass * relative_change_dot);

    // ############################################################################
    // solve overlapping
    float radius_sum = shape.radius + other_shape.radius;
    float overlap = radius_sum - distance;
    // divide the overlap by 2 so each particle
    // gets pushed the same amount in opposite directions
    overlap /= radius_sum;
    float correction = 1.1f; // NOTE: this value is here to stabilize the simulation
    Vector2 push = Vector2Normalize(impact);
    // points towards the other body
    push = Vector2Scale(push, overlap);

    other_body.position += Vector2Scale(push, other_shape.radius * correction);
    body.position -= Vector2Scale(push, shape.radius * correction);
    // // ############################################################################
}
// NOTE: use this if you want to only move smaller bodies after collision
//      and comment "overlap /= radius_sum"

// push = Vector2AddValue(push, correction);
// other_shape.radius >= shape.radius ? body.position -= push
//                                    : other_body.position += push;
// ----------------------------------------------------------------------------------

void CirclePhysicsUpdate::solve_collision(EntityId entity_id, EntityId other_entity_id) {
    // find new velocity vectors
    auto& body = global->ECS.get_component<Body>(entity_id);
    auto& other_body = global->ECS.get_component<Body>(other_entity_id);

    auto& shape = global->ECS.get_component<CircleShape>(entity_id);
    auto& other_shape = global->ECS.get_component<CircleShape>(other_entity_id);
    float distance = Vector2Distance(other_body.position, body.position);

    float mass_sum = body.mass + other_body.mass;

    Vector2 velocity_dif = other_body.velocity - body.velocity;
    Vector2 impact = other_body.position - body.position;

    float relative_change_dot = Vector2DotProduct(velocity_dif, impact);

    Vector2 unit_vector_n = Vector2ScaleDivision(impact, distance * distance * mass_sum);

    // final velocity
    body.velocity +=
        Vector2Scale(unit_vector_n, 2 * other_body.mass * relative_change_dot);

    // other body
    other_body.velocity -=
        Vector2Scale(unit_vector_n, 2 * body.mass * relative_change_dot);

    // ############################################################################
    // solve overlapping

    float radius_sum = shape.radius + other_shape.radius;
    // what i had
    // float overlap = distance - radius_sum;
    //
    float overlap = radius_sum - distance;
    // divide the overlap by 2 so each particle
    // gets pushed the same amount in opposite directions
    overlap /= radius_sum;
    float correction = 1.0f; // NOTE: this value is here to stabilize the simulation

    // points towards the other body
    Vector2 push = Vector2Normalize(impact);
    push = Vector2Scale(push, overlap);
    // push = Vector2AddValue(push, correction);
    // other_shape.radius >= shape.radius ? body.position -= push
    //                                    : other_body.position += push;

    other_body.position += Vector2Scale(push, other_shape.radius);
    body.position -= Vector2Scale(push, shape.radius);

    other_body.position = Vector2AddValue(other_body.position, correction);
    body.position = Vector2AddValue(body.position, correction);
    // other_body.position += Vector2AddValue(push, correction);
    // body.position -= Vector2AddValue(push, correction);
    // // ############################################################################
}
