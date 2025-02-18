#ifndef PLAYER_PHYSICS_HPP
#define PLAYER_PHYSICS_HPP

#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
class PlayerPhysicsRunner : System {
    // player physics runner for the player's relevant systems
    // it basically puts together the systems that concern
    // the player, and uses them to get behavior specific
    // to the player entity

  public:
    void sys_call() override { run_physics(); }
    void run_physics();
    void update_gravity(Body& player_body, CircleShape& player_shape);
    void update_position(Body& player_body);
};

class CirclePhysicsHandler : System {
  public:
    void find_gravity_field(Body& entity_body, CircleShape& entity_shape);
    void update_gravity(Body& circle_body, Body& entity_body);
};

class RectanglePhysicsHandler : System {
  public:
    void update_gravity(Body& entity_body);
};
#endif
