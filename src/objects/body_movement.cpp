// clang-format off
#include "fumo_engine/core/event_manager.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/components.hpp"
#include "raylib.h"
#include "systems.hpp"
#include "raymath.h"
#include <cerrno>
#include <sched.h>
// clang-format on

extern std::unique_ptr<GlobalState> global;

// const static float movement_scaling = 20000.0f;
const static float movement_scaling = 70000.0f;
const static float jump_scaling = 25000.0f;

void BodyMovement::move_towards(Body& body, Body& target) {
    Vector2 direction = Vector2Normalize(target.position - body.position);
    float sqr_distance = Vector2DistanceSqr(target.position, body.position);
    body.velocity = direction * sqr_distance * global->frametime;
}

void BodyMovement::move_towards_position(Body& body, Vector2 position) {
    Vector2 direction = Vector2Normalize(position - body.position);
    float sqr_distance = Vector2DistanceSqr(position, body.position);
    body.velocity = direction * sqr_distance * global->frametime;
}
// void BodyMovement::update_position(Body& body) {
//     body.position += body.velocity * global->frametime;
// }

void BodyMovement::reset_velocity(Body& body) { body.velocity = {0.0f, 0.0f}; }

// NOTE: moves the body in a fixed way, regardless of
// the velocity it had before this update
// if you want it to interact with the world, use the non fixed method
void BodyMovement::move_vertically(Body& body, float amount) {
    body.velocity -=
        body.gravity_direction * amount * movement_scaling * global->frametime;
}
void BodyMovement::move_horizontally(Body& body, float amount) {
    // if (body.rotation > 90 && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
    //     body.velocity -=
    //         body.x_direction * amount * movement_scaling * global->frametime;
    //     return;
    // }
    // PRINT(body.rotation)
    if (body.inverse_direction == true) {
        body.velocity -=
            body.x_direction * amount * movement_scaling * global->frametime;
        return;
    }
    body.velocity += body.x_direction * amount * movement_scaling * global->frametime;
}

void BodyMovement::jump(Body& body) {

    // TODO:
    // Bonus Air Time
    // Peak control
    // Fast Falling
    // Lerp the jump movement

    // body.velocity = Vector2Negate(body.gravity_direction * 2000.0f);

    body.velocity +=
        Vector2Negate(body.gravity_direction) * jump_scaling * global->frametime;
    body.jumping = true;
    body.going_up = true;
    body.on_ground = false;
    // auto scheduler_ecs = global->ECS->get_system<SchedulerSystemECS>();
    // scheduler_ecs->awake_system<JumpPhysicsHandler>();

    // NOTE: finally need the system awake thing naisu
}

bool JumpPhysicsHandler::hard_coded_jump() {
    // NOTE: this code is for testing and will be removed later

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    if (player_body.jumping) {
        // going up smoothing
        if (player_body.going_up) {
            player_body.iterations++;
            player_body.scale_velocity(-40.0f /
                                       (player_body.iterations * global->frametime));
            if (player_body.iterations < 10) {
                return true;
            }

            player_body.scale_velocity(-5.0f /
                                       (player_body.iterations * global->frametime));

            if (player_body.iterations == 17) {
                player_body.going_up = false;
                player_body.going_down = true;
                player_body.iterations = 0;
            }
        }
        // going down smoothing
        if (player_body.going_down) {
            // 25000 at least downwards
            player_body.iterations++;
            player_body.scale_velocity(3000.0f * player_body.iterations *
                                       global->frametime);

            if (player_body.iterations == 10) {
                player_body.jumping = false;
                player_body.going_down = false;
                player_body.iterations = 0;
                // const auto& scheduler_system =
                //     global->ECS->get_system<SchedulerSystemECS>();
                // scheduler_system->sleep_system<JumpPhysicsHandler>();
            }
        }
        return true;
    }
    return false;
}
