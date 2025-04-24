#pragma once
#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
#include "fumo_engine/components.hpp"
#include "fumo_engine/core/system_base.hpp"

// both used to check collisions for the player agaisnt the planets
class CollisionRunner: public System {
    void sys_call() override { check_collisions(); }
    void check_collisions();

  public:
    const Collision check_raycast_line(const Line& line);
};
