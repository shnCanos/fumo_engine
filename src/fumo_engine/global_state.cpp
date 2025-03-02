#include "fumo_engine/global_state.hpp"
#include "objects/factory_systems.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "objects/player_systems/player_physics.hpp"

void GlobalState::setup_game_state() {
    const auto& player_initializer_ptr = ECS->get_system<PlayerInitializer>();
    player_id = player_initializer_ptr->initialize_player();

    // const auto& planet_factory = ECS->get_system<PlanetFactory>();
    // EntityId planet_id =
    //     planet_factory->create_default_planet({screenCenter.x / 2.0f, screenCenter.y});
    //
    // const auto& gravity_updater = ECS->get_system<GravityUpdater>();
    //
    // gravity_updater->player_owning_planet = planet_id;
}
