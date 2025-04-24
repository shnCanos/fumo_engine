#include "constants/movement_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "main_functions.hpp"
extern std::unique_ptr<GlobalState> global;

void StateHandler::movement_state_handler(Body& player_body,
                                          Capsule& player_capsule,
                                          EntityState& player_state) {

    //-----------------------------------------------------------------
    // apply movement changes to the player
    if (player_state.on_ground) {
        // dont move in the GRAVITY direction while player is on the ground
        // nudge player SLIGHTLY towards planet to correct horizontal movement
        player_body.velocity = player_body.get_real_x_velocity()
            + player_body.get_real_y_velocity() * 0.05f;
    }

    float dot_vel = player_body.get_real_dot_y_velocity();
    // not sure if we want to ignore this when dashing
    if (dot_vel >= jump_speed_cap && !player_state.dashing) {
        player_body.velocity =
            FumoVec2Normalize(player_body.get_real_y_velocity())
                * jump_speed_cap
            + player_body.get_real_x_velocity();
    }
    player_body.position += player_body.velocity * global->frametime;
    player_capsule.update_capsule_positions(player_body);
    debug_player_drawing(player_capsule, player_body);

    //-----------------------------------------------------------------
    // scale down the velocity to prepare for
    // the calculations on the next frame
    if (player_state.dashing) {
        player_body.velocity = player_body.get_dash_x_velocity() * 0.75f
            + player_body.get_dash_y_velocity();
        return;
    }
    player_body.velocity = player_body.get_real_x_velocity() * 0.75f
        + player_body.get_real_y_velocity();
}
