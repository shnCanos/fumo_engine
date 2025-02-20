#ifndef PLAYER_GENERAL_SYSTEMS_HPP
#define PLAYER_GENERAL_SYSTEMS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"

class PlayerInitializer : public System {
  public:
    void sys_call() override {};
    EntityId initialize_player();
};

struct PlayerInputHandler : System {
    void sys_call() override { handle_input(); }

    void handle_input();
};

struct PlayerEndFrameUpdater : System {

    void sys_call() override { end_of_frame_update(); }
    void end_of_frame_update();
    void reset_state();
};

struct JumpBufferHandler : System {

    Body entity_body{};

    void sys_call() override { update_jump_buffer(); }
    void update_jump_buffer();
    void assign_entity_to_buffer(Body& new_entity_body) { entity_body = new_entity_body; }
};

#endif
