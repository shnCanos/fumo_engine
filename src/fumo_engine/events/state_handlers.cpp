#include "fumo_engine/core/global_state.hpp"
#include "main_functions.hpp"

extern std::unique_ptr<GlobalState> global;

void StateHandler::handle_states() {
    // update all states
    for (const auto& entity_id : sys_entities) {
        const auto& entity_state = global->ECS->get_component<EntityState>(entity_id);
        handle_state(entity_id, entity_state);
    }

    end_of_frame_update();
}

void StateHandler::handle_state(
    const EntityId& entity_id,
    const EntityState& entity_state
) {
    // FIXME: add a method for updating all EntityStates based on
    // matching each state so we get the behavior we want
}

void StateHandler::end_of_frame_update() {
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);
    // camera follows player
    // UpdateCameraCenterSmoothFollow(global->camera.get(), player_body);
    //-----------------------------------------------------------------
    // apply movement changes to the player
    player_body.x_direction = {
        player_body.gravity_direction.y,
        -player_body.gravity_direction.x
    };
    player_body.rotation =
        std::atan2(player_body.x_direction.y, player_body.x_direction.x) * RAD2DEG;

    if (player_state.on_ground) {
        // dont move in the GRAVITY direction while player is on the ground
        // nudge player SLIGHTLY towards planet to correct horizontal movement
        player_body.velocity =
            player_body.get_x_velocity() + player_body.get_y_velocity() * 0.1f;
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
    player_body.velocity = {0.0f, 0.0f};
}
