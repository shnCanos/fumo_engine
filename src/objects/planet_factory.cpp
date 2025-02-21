#include "constants.hpp"
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "objects/factory_systems.hpp"
#include "raymath.h"

extern std::unique_ptr<GlobalState> global;
const float default_grav_strength = 8000.0f;

EntityId PlanetFactory::create_planet(float radius, float mass, Vector2 velocity,
                                      Vector2 position, Color color, float grav_radius,
                                      float grav_strength) {
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(entity_id,
                                     Body{.position = position, .velocity = velocity});
    global->ECS->entity_add_component(entity_id, Render{.color = color});
    global->ECS->entity_add_component(entity_id, CircleShape{.radius = radius});

    global->ECS->entity_add_component(
        entity_id,
        GravityField{.gravity_radius = grav_radius, .gravity_strength = grav_strength});

    return entity_id;
}
EntityId PlanetFactory::create_default_planet(Vector2 position) {
    SetRandomSeed(time(NULL));
    Color random_color = all_colors[GetRandomValue(0, color_count - 1)];
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = Vector2Zero()});
    global->ECS->entity_add_component(entity_id, Render{.color = random_color});
    global->ECS->entity_add_component(entity_id, CircleShape{.radius = default_radius});

    global->ECS->entity_add_component(
        entity_id, GravityField{.gravity_radius = default_radius * 10.0f,
                                .gravity_strength = default_grav_strength});

    sys_entities.insert(entity_id);
    return entity_id;
}

void PlanetFactory::delete_planet(EntityId entity_id) {
    // here to keep track of what this instance of planet factory
    // has created so we can isolate these entities later
    // useful for grouping up entities based on their used context
    global->ECS->destroy_entity(entity_id);
    sys_entities.erase(entity_id);
}

void PlanetFactory::delete_all_planets() {
    for (EntityId entity_id : sys_entities) {
        global->ECS->destroy_entity(entity_id);
        sys_entities.erase(entity_id);
    }
}
