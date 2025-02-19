#ifndef SYSTEM_BASE_HPP
#define SYSTEM_BASE_HPP
#include "engine_constants.hpp"
#include <set>
// NOTE: all systems should inherit from this system

// NOTE: DONT FORGET TO INHERIT PUBLICLY FROM THIS CLASS
// and also override sys_call();
class System {
  public:
    std::set<EntityId> sys_entities{};
    // sys_call is a generic method, required to sort systems by priority at the end.
    // it is necessary to work around this method if needed for subsequent function calls
    virtual void sys_call() = 0;
    // NOTE: new variable, used to disable and enable systems
    bool awake;
};

#endif
