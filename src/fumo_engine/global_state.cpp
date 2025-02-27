#include "fumo_engine/global_state.hpp"
#include "objects/player_systems/player_general_systems.hpp"

void GlobalState::setup_game_state() {
        const auto& player_initializer_ptr = ECS->get_system<PlayerInitializer>();
        player_id = player_initializer_ptr->initialize_player();
    }
