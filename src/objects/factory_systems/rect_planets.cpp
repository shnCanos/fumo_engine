#include "constants/planet_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/screen_components.hpp"
#include "objects/factory_systems/factory_systems.hpp"
extern std::unique_ptr<GlobalState> global;

[[nodiscard]] FumoRect make_default_field_rect(FumoVec2 position);
[[nodiscard]] FumoRect make_default_ground_rect(FumoVec2 position);

EntityId LevelEntityFactory::create_screen_transition(FumoVec2 position) {

    EntityId entity_id = global->ECS->create_entity();

    ScreenTransitionRect screen_transition_rect {
        .transition_rect = make_default_ground_rect(position)};

    global->ECS->entity_add_component(entity_id, screen_transition_rect);

    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_BROWN});

    global->ECS->entity_add_component(
        entity_id,
        Body {.position = {screen_transition_rect.transition_rect.x,
                           screen_transition_rect.transition_rect.y},
              .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect(FumoVec2 position) {
    EntityId entity_id = global->ECS->create_entity();

    FumoRect ground_fumo_rect = make_default_ground_rect(position);
    global->ECS->entity_add_component(entity_id, ground_fumo_rect);

    global->ECS->entity_add_component(entity_id, ColliderObjectFlag {});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_RED});

    global->ECS->entity_add_component(
        entity_id,
        Body {.position = {ground_fumo_rect.x, ground_fumo_rect.y},
              .velocity = {0.0f, 0.0f}});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {0});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_field(FumoVec2 position) {
    EntityId entity_id = global->ECS->create_entity();

    FumoRect grav_field_fumo_rect = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id,
        ParallelGravityField {
            .field_fumo_rect = grav_field_fumo_rect,
            .gravity_strength = GRAV_STRENGTH,
        });

    global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_ORANGE});
    global->ECS->entity_add_component(
        entity_id,
        Body {.position = {grav_field_fumo_rect.x, grav_field_fumo_rect.y},
              .velocity = {0.0f, 0.0f}});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {0});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_field(FumoVec2 position,
                                               FumoVec2 grav_direction) {
    EntityId entity_id = global->ECS->create_entity();

    FumoRect grav_field_fumo_rect = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id,
        ParallelGravityField {
            .field_fumo_rect = grav_field_fumo_rect,
            .gravity_direction = grav_direction,
            .gravity_strength = GRAV_STRENGTH,
        });

    global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_ORANGE});
    global->ECS->entity_add_component(
        entity_id,
        Body {.position = {grav_field_fumo_rect.x, grav_field_fumo_rect.y},
              .velocity = {0.0f, 0.0f}});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {0});

    sys_entities.insert(entity_id);

    return entity_id;
}

[[nodiscard]] FumoRect make_default_field_rect(FumoVec2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = RECT_WIDTH,
            .height = RECT_HEIGHT};
}

[[nodiscard]] FumoRect make_default_ground_rect(FumoVec2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = RECT_WIDTH,
            .height = RECT_HEIGHT};
}

// TODO: remove these 2 functions later
// --------------------------------------------------------------------
EntityId LevelEntityFactory::create_rect_planet(FumoVec2 position) {

    EntityId entity_id = create_rect(position);
    debug__internal__create_rect_field(
        {.x = position.x, .y = position.y - RECT_HEIGHT});

    return entity_id;
}

EntityId
LevelEntityFactory::debug__internal__create_rect_field(FumoVec2 position) {
    EntityId entity_id = global->ECS->create_entity();

    FumoRect grav_field_fumo_rect = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id,
        ParallelGravityField {
            .field_fumo_rect = grav_field_fumo_rect,
            .gravity_strength = GRAV_STRENGTH,
        });

    global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_ORANGE});
    global->ECS->entity_add_component(
        entity_id,
        Body {.position = {grav_field_fumo_rect.x, grav_field_fumo_rect.y},
              .velocity = {0.0f, 0.0f}});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {0});

    sys_entities.insert(entity_id);

    return entity_id;
}
