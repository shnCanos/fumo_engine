#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "raymath.h"
#include "systems.hpp"
#include <cerrno>
#include <sched.h>

extern std::unique_ptr<GlobalState> global;

// const static float movement_scaling = 20000.0f;
const static float movement_scaling = 560.0f;
const static float jump_scaling = 830.0f;

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

void BodyMovement::reset_velocity(Body& body) { body.velocity = {0.0f, 0.0f}; }

// NOTE: moves the body in a fixed way, regardless of
// the velocity it had before this update
// if you want it to interact with the world, use the non fixed method
void BodyMovement::move_vertically(Body& body, float amount) {
    body.velocity -= body.gravity_direction * amount * movement_scaling;
}

void BodyMovement::move_horizontally(Body& body, float amount) {
    if (body.inverse_direction == true) {
        body.velocity -= body.x_direction * amount * movement_scaling;
        return;
    }
    body.velocity += body.x_direction * amount * movement_scaling;
}

float scaling = -980.0f * 3;
float default_scaling = -980.0f * 3;
float going_down_scaling = -980.0f;
float default_going_down_scaling = -980.0f;
void BodyMovement::jump(Body& body) {
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    // TODO:
    // Bonus Air Time
    // Peak control
    // Fast Falling
    // Lerp the jump movement

    if (player_state.jumping) {
        // allow player to jump right after touching the ground
        // (reset previous jump if we touch ground again and jump)
        scaling = default_scaling;
        going_down_scaling = default_going_down_scaling;
        body.iterations = 0;
    }

    body.velocity += Vector2Negate(body.gravity_direction) * jump_scaling;
    player_state.jumping = true;
    player_state.on_ground = false;
}

void BodyMovement::hard_coded_jump() {
    // NOTE: this code is for testing and will be removed later

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    // going up smoothing
    if (player_state.jumping) {
        player_body.iterations++;

        if (player_body.iterations < 6) {
            player_body.scale_velocity(scaling);
            scaling /= 1.1f;
        } else if (player_body.iterations < 24) {
            player_body.scale_velocity(scaling);
            scaling /= 1.1f;
        }

        if (player_body.iterations >= 24) {

            player_body.scale_velocity(going_down_scaling);
            going_down_scaling /= 1.2f;

            if (player_body.iterations == 46) {

                player_body.iterations = 0;
                player_state.jumping = false;
                scaling = default_scaling;
                going_down_scaling = default_going_down_scaling;
            }
            return;
        }
        if (player_body.get_dot_y_velocity() < 0) {
            // used up to iteration 24
            // we stopped going upwards
            if (scaling > going_down_scaling) scaling = going_down_scaling;
        }
    }
}
