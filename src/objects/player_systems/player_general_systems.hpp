#pragma once
#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/system_base.hpp"

namespace PlayerInitializer {
EntityId initialize_player();
void load_player_textures();
}; // namespace PlayerInitializer

struct PlayerInputHandler: System {
    void sys_call() override {
        handle_input();
    }

    void handle_input();
    void process_input(int key);
};

