#include "fumo_engine/core/global_state.hpp"
#include "objects/level_systems/input_handling_systems.hpp"
#include "objects/generic_systems/factory_systems.hpp"
#include "objects/generic_systems/systems.hpp"

const int default_rect_width = 500.0f;
const int default_rect_height = 300.0f;
extern std::unique_ptr<GlobalState> global;

void DebugLevelEditor::reset_position() {
    auto& body = global->ECS->get_component<Body>(global->player_id);
    body.position = screenCenter;
}
void DebugLevelEditor::delete_planet(Vector2 mouse_position) {
    for (const auto& entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        float distance = Vector2Distance(body.position, mouse_position);

        if (distance < mouse_radius) {

            const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
            planet_factory->delete_planet(entity_id);
            return;
        }
    }
}

void DebugLevelEditor::spawn_circular_planet(Vector2 mouse_position) {

    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_circular_planet(mouse_position);
}

void DebugLevelEditor::spawn_rect_planet(Vector2 mouse_position) {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_rect_planet(mouse_position);
}

void DebugLevelEditor::spawn_rect(Vector2 mouse_position) {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_rect(mouse_position);
}

void DebugLevelEditor::spawn_rect_field(Vector2 mouse_position) {

    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_rect_field(mouse_position);
}

void DebugLevelEditor::debug_print() {
    auto debugger_ptr = global->ECS->get_system<Debugger>();
    debugger_ptr->global_debug();
}

void DebugLevelEditor::move_entity(Vector2 mouse_position) {
    for (const auto& entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        float distance;
        EntityQuery circle_grav_query{
            .component_mask = global->ECS->make_component_mask<CircularGravityField>(),
            .component_filter = Filter::All};
        EntityQuery parallel_grav_query{
            .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
            .component_filter = Filter::All};
        EntityQuery only_grav_query{
            .component_mask = global->ECS->make_component_mask<ColliderObjectFlag>(),
            .component_filter = Filter::None};

        distance = Vector2Distance(body.position, mouse_position);

        if (distance < mouse_radius) {
            body.position = mouse_position;

            if (global->ECS->filter(entity_id, circle_grav_query)) {
                auto& gravity_field =
                    global->ECS->get_component<CircularGravityField>(entity_id);
                gravity_field.position = mouse_position;

            } else if (global->ECS->filter(entity_id, only_grav_query)) {
                // for the isolated grav fields
                auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);
                parallel_field.field_rectangle.x = mouse_position.x;
                parallel_field.field_rectangle.y = mouse_position.y;
                return;

            } else if (global->ECS->filter(entity_id, parallel_grav_query)) {
                auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);
                parallel_field.field_rectangle.x = mouse_position.x;
                parallel_field.field_rectangle.y =
                    mouse_position.y - parallel_field.field_rectangle.height;
            }
            return;
        }
    }
}

void DebugLevelEditor::handle_input() {
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);
    // DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        move_entity(mouse_position);
        return;
    } else if (IsKeyPressed(KEY_P)) {
        reset_position();

    } else if (IsKeyPressed(KEY_F1)) {
        spawn_circular_planet(mouse_position);
        return;
    } else if (IsKeyPressed(KEY_D)) {
        delete_planet(mouse_position);
    }

    mouse_position = {.x = mouse_position.x - default_rect_width / 2.0f,
                      .y = mouse_position.y - default_rect_height / 2.0f};
    // so we spawn the center of the rect on the mouse
    if (IsKeyPressed(KEY_F2)) {
        spawn_rect_planet(mouse_position);
    } else if (IsKeyPressed(KEY_F3)) {
        spawn_rect(mouse_position);
    } else if (IsKeyPressed(KEY_F4)) {
        spawn_rect_field(mouse_position);

    } else if (IsKeyPressed(KEY_V)) {
        // debug thing for fixing bad code (lands player)
        auto& body = global->ECS->get_component<Body>(global->player_id);
        body.on_ground = true;

    } else if (IsKeyPressed(KEY_ONE)) {
        debug_print();

    } else if (IsKeyPressed(KEY_R)) {
        // resize_planet(1.25f);
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_D)) {
            // add deleter methods
        } else if (IsKeyPressed(KEY_R)) {
            // resize_planet(0.80f);
        }
    }
}
