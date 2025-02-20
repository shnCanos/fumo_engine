#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"

extern std::unique_ptr<GlobalState> global;

EntityId PlayerInitializer::initialize_player() {
    EntityId player_id = global->ECS.create_entity();
    global->ECS.entity_add_component(player_id, PlayerFlag{});
    global->ECS.entity_add_component(player_id, Render{.color = player_color});
    global->ECS.entity_add_component(player_id, CircleShape{.radius = default_radius});
    global->ECS.entity_add_component(player_id,
                                     Body{.position = screenCenter, .velocity = {0.0f, 0.0f}});
    return player_id;
}
