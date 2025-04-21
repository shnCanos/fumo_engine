#include "constants/planet_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/screen_components.hpp"
#include "objects/generic_systems/factory_systems.hpp"

extern std::unique_ptr<GlobalState> global;
[[nodiscard]] FumoRect make_default_field_rect(FumoVec2 position);
[[nodiscard]] FumoRect make_default_ground_rect(FumoVec2 position);

// TODO: move these to constants when you figure out their values

void LevelEntityFactory::delete_planet(EntityId entity_id) {
    // here to keep track of what this instance of planet factory
    // has created so we can isolate these entities later
    // useful for grouping up entities based on their used context
    global->ECS->destroy_entity(entity_id);
    sys_entities.erase(entity_id);
    // TODO: create a player_owning component,
    // and have gravity updater query for that instead.
    // this is way too coupled by saving it outside of the ECS
    // like we are doing right now
    // const auto& gravity_updater = global->ECS->get_system<GravityUpdater>();
    auto& player_state =
        global->ECS->get_component<EntityState>(global->player_id);
    if (player_state.player_owning_field == entity_id) {
        player_state.player_owning_field = NO_ENTITY_FOUND;
    }
}

void LevelEntityFactory::delete_all_planets() {
    for (EntityId entity_id : sys_entities) {
        global->ECS->destroy_entity(entity_id);
    }
    sys_entities.clear();
}

EntityId LevelEntityFactory::create_circular_planet(FumoVec2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, ColliderObjectFlag {});
    global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    global->ECS->entity_add_component(
        entity_id,
        Body {.position = position, .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render {.color = FUMO_BLUE});
    global->ECS->entity_add_component(entity_id,
                                      Circle {.radius = default_radius * 4});
    global->ECS->entity_add_component(
        entity_id,
        CircularGravityField {.gravity_radius = default_planet_radius * 3,
                              .gravity_strength = default_grav_strength,
                              .position = position});

    global->ECS->entity_add_component(entity_id, Screen {0});
    global->ECS->entity_add_component(entity_id, LevelId {1});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_planet(FumoVec2 position) {

    EntityId entity_id = create_rect(position);
    create_rect_field({.x = position.x, .y = position.y - default_rect_height});
    // EntityId entity_id = global->ECS->create_entity();
    //
    // FumoRect ground_fumo_rect = make_default_ground_rect(position);
    // global->ECS->entity_add_component(entity_id, ground_fumo_rect);
    //
    // // placed above the ground fumo_rect, points downwards
    // FumoRect grav_field_fumo_rect = make_default_field_rect(position);
    // grav_field_fumo_rect.y -= default_rect_height;
    // global->ECS->entity_add_component(
    //     entity_id,
    //     ParallelGravityField {
    //         .field_fumo_rect = grav_field_fumo_rect,
    //         .gravity_strength = default_grav_strength,
    //     });
    //
    // global->ECS->entity_add_component(
    //     entity_id,
    //     Body {.position = {ground_fumo_rect.x, ground_fumo_rect.y},
    //           .velocity = {0.0f, 0.0f}});
    // global->ECS->entity_add_component(entity_id, Render {.color = FUMO_RED});
    // global->ECS->entity_add_component(entity_id, ColliderObjectFlag {});
    // global->ECS->entity_add_component(entity_id, GravFieldFlag {});
    //
    // global->ECS->entity_add_component(entity_id, Screen {0});
    // global->ECS->entity_add_component(entity_id, LevelId {0});
    //
    // sys_entities.insert(entity_id);

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

[[nodiscard]] FumoRect make_default_field_rect(FumoVec2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = default_rect_width,
            .height = default_rect_height};
}

[[nodiscard]] FumoRect make_default_ground_rect(FumoVec2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = default_rect_width,
            .height = default_rect_height};
}

EntityId LevelEntityFactory::create_rect_field(FumoVec2 position) {
    EntityId entity_id = global->ECS->create_entity();

    FumoRect grav_field_fumo_rect = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id,
        ParallelGravityField {
            .field_fumo_rect = grav_field_fumo_rect,
            .gravity_strength = default_grav_strength,
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
            .gravity_strength = default_grav_strength,
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
