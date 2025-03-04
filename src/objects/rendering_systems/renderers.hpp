#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/system_base.hpp"

struct PlanetRenderer : System {
  public:
    void sys_call() override { draw_planets(); };
    void draw_planets();
    // void draw_planet_no_gravity(EntityId entity_id);
    // void draw_planet_gravity(EntityId entity_id);
};


#endif
