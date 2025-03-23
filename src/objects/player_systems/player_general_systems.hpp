#ifndef PLAYER_GENERAL_SYSTEMS_HPP
#define PLAYER_GENERAL_SYSTEMS_HPP
#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/system_base.hpp"

class PlayerInitializer : public System {
  public:
    void sys_call() override {};
    EntityId initialize_player();
    void load_player_textures();
};

struct PlayerInputHandler : System {
    void sys_call() override { handle_input(); }

    void handle_input();
    void process_input(int key);
};

#endif
