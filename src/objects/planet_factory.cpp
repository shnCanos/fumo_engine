#include "fumo_engine/core/global_state.hpp"
#include "objects/factory_systems.hpp"

extern std::unique_ptr<GlobalState> global;
// TODO: move these to constants when you figure out their values
const float default_grav_strength = 9.8f * 6;
const float default_gravity_reach = 150.0f;
const int default_rect_width = 300.0f;
const int default_rect_height = 150.0f;

void LevelEntityFactory::delete_planet(EntityId entity_id) {
    // here to keep track of what this instance of planet factory
    // has created so we can isolate these entities later
    // useful for grouping up entities based on their used context
    global->ECS->destroy_entity(entity_id);
    sys_entities.erase(entity_id);
}

void LevelEntityFactory::delete_all_planets() {
    for (EntityId entity_id : sys_entities) {
        global->ECS->destroy_entity(entity_id);
    }
    sys_entities.clear();
}

EntityId LevelEntityFactory::create_circular_planet(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, LevelObjectFlag{});
    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render{.color = BLUE});
    global->ECS->entity_add_component(entity_id, Circle{.radius = default_radius * 5});
    global->ECS->entity_add_component(
        entity_id, CircularGravityField{.gravity_radius = default_planet_radius * 2 +
                                                          default_radius * 5,
                                        .gravity_strength = default_grav_strength});
    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_planet(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, LevelObjectFlag{});
    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    Rectangle ground_rectangle{.x = position.x + default_rect_width / 2.0f,
                               .y = position.y + default_rect_height / 2.0f,
                               .width = default_rect_width,
                               .height = default_rect_height};
    // placed above the ground rectangle, points downwards
    Rectangle grav_field_rectangle{.x = ground_rectangle.x,
                                   .y = ground_rectangle.y - ground_rectangle.height,
                                   .width = ground_rectangle.width,
                                   .height = ground_rectangle.height};

    global->ECS->entity_add_component(
        entity_id, Body{.position = {position.x + default_rect_width / 2.0f,
                                     position.y + default_rect_height / 2.0f},
                        .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render{.color = RED});
    global->ECS->entity_add_component(entity_id, ground_rectangle);
    global->ECS->entity_add_component(
        entity_id, ParallelGravityField{.field_rectangle = grav_field_rectangle,
                                        .gravity_strength = default_grav_strength});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_field(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    Rectangle grav_field_rectangle{.x = position.x + default_rect_width / 2.0f,
                                   .y = position.y + default_rect_height / 2.0f,
                                   .width = default_rect_width,
                                   .height = default_rect_height};

    global->ECS->entity_add_component(
        entity_id, ParallelGravityField{.field_rectangle = grav_field_rectangle,
                                        .gravity_strength = default_grav_strength});
    global->ECS->entity_add_component(entity_id, Render{.color = BLUE});
    global->ECS->entity_add_component(
        entity_id, Body{.position = {position.x + default_rect_width / 2.0f,
                                     position.y + default_rect_height / 2.0f},
                        .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    Rectangle ground_rectangle{.x = position.x - default_rect_width / 2.0f,
                               .y = position.y + default_rect_height / 2.0f,
                               .width = default_rect_width,
                               .height = default_rect_height};

    global->ECS->entity_add_component(entity_id, LevelObjectFlag{});
    global->ECS->entity_add_component(entity_id, Render{.color = RED});
    global->ECS->entity_add_component(entity_id, ground_rectangle);
    global->ECS->entity_add_component(
        entity_id, Body{.position = {position.x + default_rect_width / 2.0f,
                                     position.y + default_rect_height / 2.0f},
                        .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}
