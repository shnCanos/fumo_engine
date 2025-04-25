#include "constants/movement_constants.hpp"
#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
#include "fumo_engine/collisions_and_physics/collision_runner.hpp"
#include "fumo_engine/core/fumo_engine.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

#define FIX_CORNER_ISSUES() \
    do { \
        player_state.colliding_with_corner = false; \
        new_velocity -= collision.normal \
            * FumoVec2DotProduct(new_velocity, collision.normal); \
        print_direction(collision.collided_capsule_side); \
        FumoVec2 inverted = {-collision.normal.y, collision.normal.x}; \
        FumoVec2 opposite_invert = {collision.normal.y, -collision.normal.x}; \
        FumoVec2 perpendicular = (player_state.switch_perpendicular_fix) \
            ? (player_body.inverse_direction ? inverted : opposite_invert) \
            : (player_body.inverse_direction ? opposite_invert : inverted); \
        player_state.dash_start = previous_pos + new_velocity; \
        player_state.dash_end = \
            player_state.dash_start + perpendicular * (collision.distance); \
    } while (0)

void StateHandler::dash_state_handler(Body& player_body,
                                      Capsule& player_capsule,
                                      AnimationInfo& player_animation,
                                      EntityState& player_state) {

    if (player_state.dashing) {
        player_state.can_jump = false;
        player_state.jumping = false;

        player_state.dash_time += fumo_engine->frametime;
        float pos_progress = player_state.dash_time / DASH_DURATION;

        if (pos_progress > 1) pos_progress = 1;

        pos_progress = ease_quad_out(pos_progress);

        const FumoVec2& previous_pos = player_body.position;

        FumoVec2 new_pos = player_state.dash_start
            + (player_state.dash_end - player_state.dash_start) * pos_progress;

        const auto& collision_runner =
            fumo_engine->ECS->get_system<CollisionRunner>();
        const Collision& collision = collision_runner->check_raycast_line(
            {.start = previous_pos, .end = new_pos});
        FumoVec2 new_velocity = new_pos - previous_pos;

        if (collision.collided) {
            // if (player_state.colliding_with_corner) {
            //     FIX_CORNER_ISSUES();
            // } else {
            player_state.dash_end += collision.normal * (collision.overlap);
            // }
        }

        player_body.velocity = new_velocity / fumo_engine->frametime;

        // PRINT(collision.collided)
        // BeginMode2D(*fumo_engine->camera);
        // FumoDrawLineEx(previous_pos, new_pos, 20.0f, FUMO_GOLD);
        // FumoDrawCircleV(collision.intersection_point, 200.0f, FUMO_GREEN);

        // FumoDrawLineEx(player_state.dash_start,
        //                player_state.dash_end,
        //                50.0f,
        //                FUMO_PINK);
        // EndMode2D();

        if (player_state.dash_time > DASH_DURATION)
            player_state.dashing = false;

        if (player_animation.frame_progress
            != player_animation.sprite_frame_count) {
            AnimationPlayer::play(player_animation, "dash");
        }
    }

    else {
        player_body.dash_x_direction = {player_body.dash_gravity_direction.y,
                                        -player_body.dash_gravity_direction.x};
    }
}
