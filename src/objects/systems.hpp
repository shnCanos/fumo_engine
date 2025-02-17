#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include "raylib.h"

// TODO: BodyMovement shouldnt be registered? (undecided atm)
// SOLVED: created an unscheduled option in Scheduler
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
};

class InputHandler : public System {
    // generic interface aggregate for all input handling
  public:
    virtual ~InputHandler() = default;
    void sys_call() override = 0;
};

class InputHandlerLevelEditor : public InputHandler {
  public:
    void sys_call() override { handle_input(); }
    void handle_input();

  private:
    void spawn_planet();
    void move_planet();
    void delete_planet();
    void delete_all_planets();
    void resize_planet(float resize);
    void debug_print();
};

// NOTE: this is an interface for all factory spawner classes
class EntityFactory : public System {
    // this factory method will hold a bunch of functions for spawning various entities
    // maybe we will give it some parameters so we can easily spawn many types of entities
  public:
    virtual ~EntityFactory() = default;
};
class PlanetFactory : public EntityFactory {
  public:
    void sys_call() override {};
    EntityId create_default_planet(Vector2 position);
    EntityId create_planet(float radius, float mass, Vector2 velocity, Vector2 position,
                           Color color);
};

class Renderer : public System {
  public:
    virtual ~Renderer() = default;
};
class PlanetRenderer : public Renderer {
  public:
    void sys_call() override { draw_planet(); };
    void draw_planet();
};

class Debugger : public System {
    // not very used for now
    // TODO: expand this class to allow for many kinds of debugging functions
    // and common analysis of game state (like printing entities every 10 seconds)
    // (or like targetting specific parts of the ECS)

    void sys_call() override { global_debug(); };
    void global_debug();
};

#endif
