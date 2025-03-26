#include <sched.h>

#include <cerrno>

#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "raymath.h"
#include "systems.hpp"

extern std::unique_ptr<GlobalState> global;

// const static float movement_scaling = 20000.0f;
const static float movement_scaling = 560.0f;
const static float jump_scaling = 400.0f;

void BodyMovement::move(const EntityId& entity_id, const DIRECTION& direction) {
    auto& body = global->ECS->get_component<Body>(entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(entity_id);

    switch (direction) {
        case DIRECTION::LEFT:
            body.velocity -= body.x_direction * movement_scaling;
            if (player_state.on_ground) {
                body.inverse_direction = true;
                AnimationPlayer::play(player_animation, "run");
            }
            break;
        case DIRECTION::RIGHT:
            body.velocity += body.x_direction * movement_scaling;
            if (player_state.on_ground) {
                body.inverse_direction = false;
                AnimationPlayer::play(player_animation, "run");
            }
            break;

        default:
            PANIC("UNREACHABLE");
            std::unreachable();
    }
}

float scaling = -980.0f * 3;
float default_scaling = -980.0f * 3;
float going_down_scaling = -900.0f;
float default_going_down_scaling = -900.0f;

void BodyMovement::jump(Body& body, const EntityId& entity_id) {
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);

    // TODO:
    // Bonus Air Time
    // Peak control
    // Fast Falling
    // Lerp the jump movement

    global->event_handler->add_event({EVENT_::ENTITY_JUMPED, entity_id});

    scaling = default_scaling;
    going_down_scaling = default_going_down_scaling;

    body.velocity += Vector2Negate(body.gravity_direction) * jump_scaling;
}

void BodyMovement::hard_coded_jump() {
    // NOTE: this code is for testing and will be removed later

    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);
    auto& player_body = global->ECS->get_component<Body>(global->player_id);

    // if (player_state.jumping) {
    //     if(IsKeyDown(KEY_SPACE)) {
    //         player_body.velocity
    //
    //     }
    //
    // }

    if (IsKeyReleased(KEY_SPACE)) {
        if (player_body.iterations < 30) {
            player_body.iterations = 40;

            going_down_scaling = scaling;
        }
        // player_state.jumping = false;
        // scaling = default_scaling;
    }

    // going up smoothing
    if (player_state.jumping) {
        auto& player_body = global->ECS->get_component<Body>(global->player_id);
        player_body.iterations++;

        if (player_body.iterations < 6) {
            player_body.scale_velocity(scaling);
            scaling /= 1.1f;
            return;
        }
        if (player_body.iterations < 24) {
            player_body.scale_velocity(scaling);
            scaling /= 1.08f;
            return;
        }

        if (player_body.iterations < 30) {
            player_body.scale_velocity(going_down_scaling);
            going_down_scaling /= 1.15f;
            return;
        }
        if (player_body.iterations < 50) {
            player_body.scale_velocity(going_down_scaling);
            going_down_scaling /= 1.24f;
            return;
        }
        player_body.iterations = 0;
        player_state.jumping = false;
        scaling = default_scaling;
        going_down_scaling = default_going_down_scaling;
        // if (player_body.get_dot_y_velocity() < 0) {
        //     // used up to iteration 24
        //     // we stopped going upwards
        //     if (scaling > going_down_scaling)
        //         scaling = going_down_scaling;
        // }
    }
}

// void move_horizontally(Body& body, float amount) {
//     if (body.inverse_direction == true) {
//         body.velocity -= body.x_direction * amount * movement_scaling;
//         return;
//     }
//     body.velocity += body.x_direction * amount * movement_scaling;
// }

//
// void move_vertically(Body& body, float amount) {
//     body.velocity -= body.gravity_direction * amount * movement_scaling;
// }
// void BodyMovement::move_towards(Body& body, Body& target) {
//     Vector2 direction = Vector2Normalize(target.position - body.position);
//     float sqr_distance = Vector2DistanceSqr(target.position, body.position);
//     body.velocity = direction * sqr_distance * global->frametime;
// }
//
// void BodyMovement::move_towards_position(Body& body, Vector2 position) {
//     Vector2 direction = Vector2Normalize(position - body.position);
//     float sqr_distance = Vector2DistanceSqr(position, body.position);
//     body.velocity = direction * sqr_distance * global->frametime;
// }
