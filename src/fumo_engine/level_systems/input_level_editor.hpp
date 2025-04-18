#pragma once
#include "fumo_engine/core/system_base.hpp"
#include "fumo_raylib.hpp"

class DebugLevelEditor: public System {

  public:
    void sys_call() override {
        handle_input();
    }

    void handle_input();
    void spawn_rect_planet(FumoVec2 mouse_position);
    void spawn_circular_planet(FumoVec2 mouse_position);
    void spawn_rect_field(FumoVec2 mouse_position);
    void spawn_rect(FumoVec2 mouse_position);
    void debug_print();
    void move_entity(FumoVec2 mouse_position);
    void resize_planet(float resize);
    void delete_planet(FumoVec2 mouse_position);
    void reset_position();
};

struct LevelEditorGUI {
    void save_level();
};
