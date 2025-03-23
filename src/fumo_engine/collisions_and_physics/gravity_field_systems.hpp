#ifndef PLAYER_PHYSICS_HPP
#define PLAYER_PHYSICS_HPP

#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/components.hpp"

struct GravityBufferHandler : System {

    void sys_call() override { wait_for_touching_ground(); }

    void wait_for_touching_ground();
};

struct GravityUpdater : System {

    // EntityId player_owning_planet = NO_ENTITY_FOUND;

    void sys_call() override { gravity_update(); }
    void gravity_update();
    void update_gravity(Body& player_body);
    void update_position(Body& player_body);
};

struct GravityFieldHandler : System {
    // only cares about entities on screen

    void sys_call() override { find_gravity_field(); }

    void find_gravity_field();
    void pick_final_field(const std::vector<EntityId>& candidate_planets,
                          const Body& player_body);
};


#endif
