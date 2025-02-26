#include "fumo_engine/global_state.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;


void GlobalRenderer::global_texture_load() { 

    const auto& player_initializer = global->ECS->get_system<PlayerInitializer>();

    player_initializer->load_player_textures();



}






