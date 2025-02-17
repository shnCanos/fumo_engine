#include "constants.hpp"
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "raymath.h"
#include "systems.hpp"
extern std::unique_ptr<GlobalState> global;
EntityId PlanetFactory::create_planet(float radius, float mass, Vector2 velocity,
                                      Vector2 position, Color color) {
    EntityId entity_id = global->ECS.create_entity();
    global->ECS.entity_add_component(
        entity_id, Body{.position = position, .velocity = velocity, .mass = mass});
    global->ECS.entity_add_component(entity_id, Render{.color = color});
    global->ECS.entity_add_component(entity_id, CircleShape{.radius = radius});
    return entity_id;
}
EntityId PlanetFactory::create_default_planet(Vector2 position) {
    SetRandomSeed(time(NULL));
    Color random_color = all_colors[GetRandomValue(0, color_count - 1)];
    EntityId entity_id = global->ECS.create_entity();
    global->ECS.entity_add_component(
        entity_id,
        Body{.position = position, .velocity = Vector2Zero(), .mass = default_mass});
    global->ECS.entity_add_component(entity_id, Render{.color = random_color});
    global->ECS.entity_add_component(entity_id, CircleShape{.radius = default_radius});
    return entity_id;
}
