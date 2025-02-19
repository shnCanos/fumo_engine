#ifndef PLAYER_PHYSICS_HPP
#define PLAYER_PHYSICS_HPP

#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
class PlayerPhysicsRunner : public System {
    // player physics runner for the player's relevant systems
    // it basically puts together the systems that concern
    // the player, and uses them to get behavior specific
    // to the player entity

  public:
    void sys_call() override { run_physics(); }
    void run_physics();

  private:
    void update_gravity(Body& player_body, CircleShape& player_shape);
    void update_position(Body& player_body);
};

class CirclePhysicsHandler : public System {
  public:
    void sys_call() override {};
    void find_gravity_field(Body& entity_body, CircleShape& entity_shape);
    void update_gravity(const GravityField& circle_grav_field, const Body& circle_body,
                        Body& entity_body);
};

class RectanglePhysicsHandler : public System {
    // FIXME: add rectangle physics
    // also add rectangle gravity fields
  public:
    void sys_call() override {};
    void update_gravity(Body& entity_body);
};
#endif
