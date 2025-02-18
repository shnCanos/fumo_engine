#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP
#include "fumo_engine/engine_constants.hpp"
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
    void debug_prints() {
        ECS.debug_print();
        std::cerr << std::endl << std::endl;
    }
};

#endif
