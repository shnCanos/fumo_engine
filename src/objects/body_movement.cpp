// clang-format off
#include "fumo_engine/global_state.hpp"
#include "systems.hpp"
#include "raymath.h"
// clang-format on

extern std::unique_ptr<GlobalState> global;

const static float movement_scaling = 150.0f;

void BodyMovement::move_towards(Body& body, Body& target) {
    Vector2 direction = Vector2Normalize(target.position - body.position);
    float sqr_distance = Vector2DistanceSqr(target.position, body.position);
    body.velocity = direction * sqr_distance;
}

void BodyMovement::move_towards_position(Body& body, Vector2 position) {
    Vector2 direction = Vector2Normalize(position - body.position);
    float sqr_distance = Vector2DistanceSqr(position, body.position);
    body.velocity = direction * sqr_distance;
}
void BodyMovement::update_position(Body& body) {
    body.position += body.velocity * global->frametime;
    body.velocity = {0.0f, 0.0f};
}

void BodyMovement::move_vertically(Body& body, float amount) {
    body.velocity += body.gravity_direction * amount * movement_scaling;

    // body.velocity = Vector2Subtract(body.velocity, Vector2Scale(body.gravity_direction, amount));


}
void BodyMovement::move_horizontally(Body& body, float amount) {
    Vector2 x_direction = {-body.gravity_direction.y, body.gravity_direction.x};
    body.velocity += x_direction * amount * movement_scaling;
}
