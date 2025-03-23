#include "objects/player_systems/player_general_systems.hpp"

#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

EntityId PlayerInitializer::initialize_player() {
    EntityId player_id = global->ECS->create_entity();

    // global->ECS->entity_add_component(player_id, PlayerFlag{});
    global->ECS->entity_add_component(player_id, EntityState {.can_swap_orbits = true});
    global->ECS->entity_add_component(player_id, Render {.color = {50, 50, 50, 150}});

    global->ECS->entity_add_component(
        player_id,
        AnimationInfo {.sprite_scaling = 3.0f}
    );

    Body player_body {
        .position = {.x = screenWidth / 2.0f, .y = screenCenter.y},
        .velocity = {0.0f, 0.0f}
    };

    global->ECS->entity_add_component(player_id, player_body);

    PlayerShape player_shape {.radius = 33.0f};
    player_shape.update_capsule_positions(player_body);
    global->ECS->entity_add_component(player_id, player_shape);

    return player_id;
}
