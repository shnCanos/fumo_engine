#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP
#include "scheduler_ecs.hpp"

class GlobalState {
  public:
    // NOTE: I merged the scheduler with the ECS so we can separate game specific global
    // state i dont want to rewrite access functions so SchedulerECS is accessed directly
    float frametime;
    SchedulerECS ECS;
    void initialize() { ECS.initialize(); }
    void debug_prints() {
        ECS.debug_print();
        std::cerr << std::endl << std::endl;
    }
};

#endif
