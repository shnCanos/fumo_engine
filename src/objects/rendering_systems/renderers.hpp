#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
typedef Texture Texture2D;

struct PlanetRenderer : System {
  public:
    void sys_call() override { draw_planet(); };
    void draw_planet();
};


#endif
