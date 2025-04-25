#include "constants/planet_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/screen_components.hpp"
#include "objects/factory_systems/factory_systems.hpp"
extern std::unique_ptr<GlobalState> global;

EntityId LevelEntityFactory::create_circular_planet(FumoVec2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, ColliderObjectFlag {});
    global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    global->ECS->entity_add_component(
        entity_id,
        Body {.position = position, .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_BLUE});
    global->ECS->entity_add_component(entity_id,
                                      Circle {.radius = CIRCLE_RADIUS * 4});
    global->ECS->entity_add_component(
        entity_id,
        CircularGravityField {.gravity_radius = GRAV_REACH * 3,
                              .gravity_strength = GRAV_STRENGTH,
                              .position = position});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {1});

    sys_entities.insert(entity_id);

    return entity_id;
}
