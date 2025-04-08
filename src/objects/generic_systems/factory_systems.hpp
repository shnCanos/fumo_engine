#pragma once
#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/system_base.hpp"
#include "raylib.h"

class LevelEntityFactory : public System {
    // NOTE: planet factory now keeps track of the entities
    // it created so that we can delete only the ones
    // created by it (so we can undo input handler actions without altering
    // the rest of the planets already on screen)
  public:
    void sys_call() override {};
    void delete_planet(EntityId entity_id);
    void delete_all_planets();
    EntityId create_rect_planet(Vector2 position);
    EntityId create_circular_planet(Vector2 position);
    EntityId create_rect_field(Vector2 position);
    EntityId create_rect(Vector2 position);

    EntityId create_rect_field(Vector2 position, Vector2 grav_direction);
};

