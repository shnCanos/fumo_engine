#include "constants.hpp"
#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/components.hpp"
#include "objects/factory_systems.hpp"
#include "raymath.h"

extern std::unique_ptr<GlobalState> global;
const float default_grav_strength = 9.8f * 6;

EntityId PlanetFactory::create_planet(float radius, float mass, Vector2 velocity,
                                      Vector2 position, Color color, float grav_radius,
                                      float grav_strength) {
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(entity_id,
                                      Body{.position = position, .velocity = velocity});
    global->ECS->entity_add_component(entity_id, Render{.color = color});
    global->ECS->entity_add_component(entity_id, Circle{.radius = radius});

    global->ECS->entity_add_component(
        entity_id,
        GravityField{.gravity_radius = grav_radius, .gravity_strength = grav_strength});

    return entity_id;
}
EntityId PlanetFactory::create_default_planet(Vector2 position, Color color) {
    // SetRandomSeed(time(NULL));
    // Color random_color = all_colors[GetRandomValue(0, color_count - 1)];
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = Vector2Zero()});
    global->ECS->entity_add_component(entity_id, Render{.color = color});
    global->ECS->entity_add_component(entity_id,
                                      Circle{.radius = default_radius * 5});

    global->ECS->entity_add_component(
        entity_id, GravityField{.gravity_radius = default_planet_radius * 2,
                                .gravity_strength = default_grav_strength});

    sys_entities.insert(entity_id);
    return entity_id;
}

EntityId PlanetFactory::create_default_aggregate_field_planet(Vector2 position,
                                                             Color color) {
    // SetRandomSeed(time(NULL));
    // Color random_color = all_colors[GetRandomValue(0, color_count - 1)];
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = Vector2Zero()});
    global->ECS->entity_add_component(entity_id, Render{.color = color});
    global->ECS->entity_add_component(entity_id,
                                      Circle{.radius = default_radius * 5});

    global->ECS->entity_add_component(
        entity_id, GravityField{.gravity_radius = default_planet_radius * 2,
                                .gravity_strength = default_grav_strength});

    global->ECS->entity_add_component(entity_id, AggregateField{});

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

EntityId PlanetFactory::create_planet_no_gravity(Vector2 position, Color color) {
    EntityId entity_id = global->ECS->create_entity();
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = Vector2Zero()});
    global->ECS->entity_add_component(entity_id, Render{.color = color});
    global->ECS->entity_add_component(entity_id,
                                      Circle{.radius = default_radius * 5});
    sys_entities.insert(entity_id);
    return entity_id;
}
