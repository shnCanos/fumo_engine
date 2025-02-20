#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP
#include "fumo_engine/engine_constants.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "scheduler_ecs.hpp"

class GlobalState {
  public:
    // NOTE: I merged the scheduler with the ECS so we can separate game specific global
    // state i dont want to rewrite access functions so SchedulerECS is accessed directly

    float frametime;
    SchedulerECS ECS;
    EntityId player_id; // NOTE: storing the player id globally for now for optimisation
                        // this isnt necessary, and can be removed completely later.

    void initialize() { ECS.initialize(); }

    void setup_game_state() {
        auto player_initializer_ptr = ECS.get_system<PlayerInitializer>();
        player_id = player_initializer_ptr->initialize_player();
    }
};

#endif
