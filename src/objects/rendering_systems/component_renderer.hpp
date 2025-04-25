#pragma once
#include "fumo_engine/core/system_base.hpp"

struct ComponentRenderer: System {
  public:
    void sys_call() override { draw_all_components(); };

    void draw_all_components();
};

// struct GravFieldRenderer : System {
//   public:
//     void sys_call() override { draw_fields(); };
//     void draw_fields();
// };
