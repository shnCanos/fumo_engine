#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP
#include "fumo_engine/core/system_base.hpp"
#include "objects/components.hpp"
#include "raylib.h"

class BodyMovement : public System {
    // prefer making a body movement system, instead of
    // adding a move_towards() method to the Body component (follow ECS logic)
  public:
    // NOTE: you need to give a definition of sys_call() virtual method,
    // otherwise the compiler wont allocate a vtable for this class,
    // since it will treat it as virtual and not allow instantiations of it
    void sys_call() override {};
    void move_towards(Body& body, Body& target);
    void move_towards_position(Body& body, Vector2 position);
    // used for player
    void move_vertically_fixed(Body& body, float amount);
    void move_horizontally_fixed(Body& body, float amount);
    void move_vertically(Body& body, float amount);
    void jump(Body& body);
    void move_horizontally(Body& body, float amount);
    void update_position(Body& body);
    void reset_velocity(Body& body);
};
struct JumpPhysicsHandler : System {
    void sys_call() override {}
    bool hard_coded_jump();
};

// class InputHandler : public System {
//     // generic interface aggregate for all input handling
//   public:
//     virtual ~InputHandler() = default;
//     void sys_call() override = 0;
// };
//
// class InputHandlerLevelEditor : public InputHandler {
//   public:
//     void sys_call() override { handle_input(); }
//     void handle_input();
//
//     // TODO: rewrite this class so its split into more classwe
//   private:
//     void spawn_planet();
//     void spawn_aggregate_field_planet();
//     void move_planet();
//     void delete_created_planet();
//     void delete_all_created_planets();
//     void delete_all_planets();
//     void resize_planet(float resize);
//     void debug_print();
//     void spawn_planet_no_gravity();
// };

class Debugger : public System {
    // not very used for now
    // TODO: expand this class to allow for many kinds of debugging functions
    // and common analysis of game state (like printing entities every 10 seconds)
    // (or like targetting specific parts of the ECS)

  public:
    void sys_call() override { global_debug(); };
    void global_debug();
};

#endif
