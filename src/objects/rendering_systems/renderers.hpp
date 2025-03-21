#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/core/system_base.hpp"

struct ObjectRenderer : System {
  public:
    void sys_call() override { draw_planets(); };
    void draw_planets();
};

struct GravFieldRenderer : System {
  public:
    void sys_call() override { draw_fields(); };
    void draw_fields();
};

#endif
