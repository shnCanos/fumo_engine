#ifndef PLAYER_GENERAL_SYSTEMS_HPP
#define PLAYER_GENERAL_SYSTEMS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
class PlayerInitializer : public System {
  public:
    void sys_call() override {
    };
    EntityId initialize_player();
};
#endif
