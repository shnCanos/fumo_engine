#ifndef FACTORY_SYSTEMS_HPP
#define FACTORY_SYSTEMS_HPP
#include "fumo_engine/engine_constants.hpp"
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
    // NOTE: planet factory now keeps track of the entities
    // it created so that we can delete only the ones
    // created by it (so we can undo input handler actions without altering
    // the rest of the planets already on screen)
  public:
    void sys_call() override {};
    EntityId create_default_planet(Vector2 position);
    EntityId create_planet(float radius, float mass, Vector2 velocity, Vector2 position,
                           Color color);
    void delete_planet(EntityId entity_id);
    void delete_all_planets();
};


#endif
