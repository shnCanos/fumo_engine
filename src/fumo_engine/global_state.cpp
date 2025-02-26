#include "fumo_engine/global_state.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "objects/rendering_systems/renderers.hpp"

void GlobalState::setup_game_state() {
        const auto& player_initializer_ptr = ECS->get_system<PlayerInitializer>();
        player_id = player_initializer_ptr->initialize_player();
        const auto& global_renderer_ptr = ECS->get_system<GlobalRenderer>();
        global_renderer_ptr->global_texture_load();
    }
