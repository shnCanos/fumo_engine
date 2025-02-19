#ifndef FACTORY_SYSTEMS_HPP
#define FACTORY_SYSTEMS_HPP
#include "fumo_engine/system_base.hpp"
#include "raylib.h"

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
#endif
