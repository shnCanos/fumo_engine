#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP
#include "fumo_engine/engine_constants.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "objects/rendering_systems/renderers.hpp"
#include "scheduler_ecs.hpp"
#include <memory>

class GlobalState {
  public:
    // NOTE: I merged the scheduler with the ECS so we can separate game specific global
    // state i dont want to rewrite access functions so SchedulerECS is accessed directly

    float frametime;
    std::shared_ptr<SchedulerECS> ECS;
    EntityId player_id; // NOTE: storing the player id globally for now for optimisation
                        // this isnt necessary, and can be removed completely later.

    void initialize() {
        ECS = std::make_shared<SchedulerECS>();
        ECS->initialize();
    }

    void setup_game_state() {
        const auto& player_initializer_ptr = ECS->get_system<PlayerInitializer>();
        player_id = player_initializer_ptr->initialize_player();
        const auto& global_renderer_ptr = ECS->get_system<GlobalRenderer>();
        global_renderer_ptr->global_texture_load();
    }
    void destroy_and_unload_game() {
        // TODO: unload our textures and also destroy all dynamic memory in global
        // (so far we have no memory to clear so this is not necessary)
    }
};

#endif
