#include "constants/movement_constants.hpp"
#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
#include "fumo_engine/collisions_and_physics/collision_runner.hpp"
#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

void StateHandler::dash_state_handler(Body& player_body,
                                      Capsule& player_capsule,
                                      AnimationInfo& player_animation,
                                      EntityState& player_state) {
    // FumoVec2 new_velocity =
    //     player_body.velocity + (new_pos - player_body.position);

    if (player_state.dashing) {
        player_state.can_jump = false;
        player_state.jumping = false;

        player_state.dash_time += global->frametime;
        float pos_progress = player_state.dash_time / dash_duration;

        if (pos_progress > 1) pos_progress = 1;

        pos_progress = ease_quad_out(pos_progress);

        const FumoVec2& previous_pos = player_body.position;

        FumoVec2 new_pos = player_state.dash_start
            + (player_state.dash_end - player_state.dash_start) * pos_progress;

        const auto& collision_runner =
            global->ECS->get_system<CollisionRunner>();

        const Collision& collision = collision_runner->check_raycast_line(
            {.start = previous_pos, .end = new_pos});

        // PRINT(collision.collided)

        // BeginMode2D(*global->camera);
        // FumoDrawLineEx(previous_pos, new_pos, 20.0f, FUMO_GOLD);
        // FumoDrawCircleV(collision.intersection_point, 200.0f, FUMO_GREEN);

        FumoVec2 new_velocity = new_pos - previous_pos;

        if (collision.collided) {

            new_velocity += collision.normal_or_push * player_capsule.radius;
            new_velocity -= collision.normal_or_push
                * FumoVec2DotProduct(new_velocity, collision.normal_or_push);
        }

        player_body.velocity = new_velocity / global->frametime;

        // FumoDrawLineEx(previous_pos,
        //                previous_pos + player_body.velocity,
        //                50.0f,
        //                FUMO_PINK);
        // EndMode2D();

        if (player_state.dash_time > dash_duration)
            player_state.dashing = false;

        if (player_animation.frame_progress
            != player_animation.sprite_frame_count) {
            AnimationPlayer::play(player_animation, "dash");
        }

    } else {
        player_body.dash_x_direction = {player_body.dash_gravity_direction.y,
                                        -player_body.dash_gravity_direction.x};
    }
}
