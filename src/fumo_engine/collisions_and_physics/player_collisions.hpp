#pragma once
#include "fumo_engine/core/system_base.hpp"

// both used to check collisions for the player agaisnt the planets
class PlayerCollisionRunner : public System {
    void sys_call() override { check_collisions(); }
    void check_collisions();
};
