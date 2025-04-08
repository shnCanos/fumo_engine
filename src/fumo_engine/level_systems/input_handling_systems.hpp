#pragma once
#include "fumo_engine/core/system_base.hpp"
#include "raylib.h"

class DebugLevelEditor : public System {

  public:
    void sys_call() override { handle_input(); }

    void handle_input();
    void spawn_rect_planet(Vector2 mouse_position);
    void spawn_circular_planet(Vector2 mouse_position);
    void spawn_rect_field(Vector2 mouse_position);
    void spawn_rect(Vector2 mouse_position);
    void debug_print();
    void move_entity(Vector2 mouse_position);
    void resize_planet(float resize);
    void delete_planet(Vector2 mouse_position);
void reset_position();
};

