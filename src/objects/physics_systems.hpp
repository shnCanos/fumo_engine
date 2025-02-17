#ifndef PHYSICS_SYSTEMS_HPP
#define PHYSICS_SYSTEMS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
class PhysicsUpdate : public System {

    // plan is to kind of use this class as a way to give a better name to the
    // function call, also to group all physics updates into one known base type
    // this way they are more easily grouped later if needed
    // (this class guarantees the frametime variable in calling too)
  public:
    virtual ~PhysicsUpdate() = default;
};

class CirclePhysicsUpdate : public PhysicsUpdate {
    // an example, we deal with only the circle physics update here
    // granted, all of this could simply be one with if statements,
    // or just adding another system unrelated to physics update
    // but i want to try grouping these together for now (can remove later)

  private:
    void solve_collision_aux(Body& body, Body& other_body, CircleShape& shape,
                             CircleShape& other_shape);

  public:
    void sys_call() override {
        update_velocity();
        detect_screen_collision();
        detect_collisions();
        update_position();
    }
    void detect_screen_collision();
    void detect_collisions();
    void detect_collision(EntityId entity_id, EntityId other_entity_id);
    void solve_collision(EntityId entity_id, EntityId other_entity_id);
    void update_velocity();
    void update_position();
};
#endif
