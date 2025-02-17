// clang-format off
#include "systems.hpp"
#include "raymath.h"
// clang-format on

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
