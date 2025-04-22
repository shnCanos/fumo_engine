#include "fumo_engine/core/global_state.hpp"
#include "fumo_raylib.hpp"
#include "main_functions.hpp"
#include "constants/movement_constants.hpp"

extern std::unique_ptr<GlobalState> global;

void StateHandler::handle_states() {
    // update all states
    for (const auto& entity_id : sys_entities) {
        const auto& entity_state =
            global->ECS->get_component<EntityState>(entity_id);
        handle_state(entity_id, entity_state);
    }

    end_of_frame_update();
}

int fall_counter = 0;

void StateHandler::handle_state(const EntityId& entity_id,
                                const EntityState& entity_state) {
    auto& player_body = global->ECS->get_component<Body>(entity_id);
    auto& player_animation =
        global->ECS->get_component<AnimationInfo>(entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);
    auto& moved_event_data =
        global->ECS->get_component<MovedEventData>(entity_id);
    // if we face left, then we invert the sprite
    player_body.inverse_direction =
        (moved_event_data.continue_in_direction == DIRECTION::LEFT);

    // if (player_state.input_direction == FumoVec2 {.x = -1.0f, .y = 1.0f}
    //     || player_state.input_direction == FumoVec2 {.x = -1.0f, .y = -1.0f}) {
    //     player_body.inverse_direction = true;
    // }

    if (player_state.jumping && !player_state.dashing) {
        if (player_animation.frame_progress
            != player_animation.sprite_frame_count) {
            AnimationPlayer::play(player_animation, "jump");
        }
        return;
    }

    if (!player_state.colliding) {
        player_state.on_ground = false;
    }

    player_state.falling = !player_state.on_ground && !player_state.colliding;

    if (!player_state.idle && !player_state.dashing) {
        if (player_animation.current_sheet_name != "dash") {
            AnimationPlayer::play(player_animation, "jump");
            player_animation.frame_progress = 3;
            player_animation.current_region_rect.x =
                player_animation.current_region_rect.width
                * player_animation.frame_progress;
            player_animation.is_running = true;
        }
    }
}

// float ease_quad_out(float t) { return 1 - (1 - t) * (1 - t); }

void StateHandler::end_of_frame_update() {
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape =
        global->ECS->get_component<PlayerShape>(global->player_id);
    auto& player_state =
        global->ECS->get_component<EntityState>(global->player_id);
    auto& player_animation =
        global->ECS->get_component<AnimationInfo>(global->player_id);

    // camera follows player
    UpdateCameraCenterSmoothFollow(global->camera.get(), player_body);
    //-----------------------------------------------------------------
    // apply movement changes to the player
    // if (true || player_state.can_swap_orbits) {
    if (player_state.dashing) {
        player_state.can_jump = false;
        player_state.jumping = false;

        if (player_state.dash_time == 0) {}
        player_state.dash_time += global->frametime;
        float pos_progress = player_state.dash_time / dash_duration;

        if (pos_progress > 1) pos_progress = 1;

        pos_progress = ease_quad_out(pos_progress);

        auto new_pos = player_state.dash_start
            + (player_state.dash_end - player_state.dash_start) * pos_progress;

        // float flip = player_body.inverse_direction ? -1 : 1;
        player_body.velocity = (new_pos - player_body.position) / global->frametime;

        if (player_state.dash_time > dash_duration)
            player_state.dashing = false;

        if (player_animation.frame_progress
            != player_animation.sprite_frame_count) {
            AnimationPlayer::play(player_animation, "dash");
        }

    } else {
        player_body.x_direction = {player_body.gravity_direction.y,
                                   -player_body.gravity_direction.x};
        player_body.rotation =
            std::atan2(player_body.x_direction.y, player_body.x_direction.x)
            * RAD2DEG;
    }
    // }

    if (player_state.on_ground) {
        // dont move in the GRAVITY direction while player is on the ground
        // nudge player SLIGHTLY towards planet to correct horizontal movement
        player_body.velocity =
            player_body.get_x_velocity() + player_body.get_y_velocity() * 0.05f;
    }

    float dot_vel = player_body.get_dot_y_velocity();
    if (dot_vel >= jump_speed_cap) {
        player_body.velocity =
            FumoVec2Normalize(player_body.get_y_velocity()) * jump_speed_cap
            + player_body.get_x_velocity();
    }
    
    player_body.position += player_body.velocity * global->frametime;
    player_shape.update_capsule_positions(player_body);
    //-----------------------------------------------------------------
    // DEBUG
    debug_player_drawing(player_shape, player_body);
    //-----------------------------------------------------------------

    // if we leave the ground we should slowly get accelerated by the gravity.
    // if we swap orbits the same thing should happen
    //
    // also, we should LERP between the new gravity direction and the old one
    // and slowly rotate the player

    // if (!player_state.is_changing_screens) {
    // if (!player_state.dashing)
    player_body.velocity =
        player_body.get_x_velocity() * 0.75f + player_body.get_y_velocity();
    // }
}
