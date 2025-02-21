#include "objects/player_systems/player_general_systems.hpp"
#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "objects/systems.hpp"

extern std::unique_ptr<GlobalState> global;

EntityId PlayerInitializer::initialize_player() {
    EntityId player_id = global->ECS->create_entity();
    global->ECS->entity_add_component(player_id, PlayerFlag{});
    global->ECS->entity_add_component(player_id, Render{.color = player_color});
    global->ECS->entity_add_component(player_id,
                                     CircleShape{.radius = default_radius / 2.0f});
    global->ECS->entity_add_component(player_id, Body{.position = screenCenter,
                                                     .velocity = {0.0f, 0.0f},
                                                     .smooth_jump_buffer = 800.0f});
    return player_id;
}

void PlayerEndFrameUpdater::end_of_frame_update() { reset_state(); }

void PlayerEndFrameUpdater::reset_state() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    const auto& body_movement_ptr = global->ECS->get_system<BodyMovement>();

    body_movement_ptr->reset_velocity(player_body);
}
