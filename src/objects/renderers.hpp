#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/system_base.hpp"

class Renderer : public System {
  public:
    virtual ~Renderer() = default;
};
class PlanetRenderer : public Renderer {
  public:
    void sys_call() override { draw_planet(); };
    void draw_planet();
};
#endif
