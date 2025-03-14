// clang-format off
#include "fumo_engine/core/global_state.hpp"
#include "input_handling_systems.hpp"
#include "objects/factory_systems.hpp"
#include "objects/systems.hpp"
// clang-format on

extern std::unique_ptr<GlobalState> global;

void DebugLevelEditor::spawn_rect_planet(Vector2 mouse_position) {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_rect_planet(mouse_position);
}

void DebugLevelEditor::spawn_circular_planet(Vector2 mouse_position) {

    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
    planet_factory->create_circular_planet(mouse_position);
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
        body.position = mouse_position;
        return;
    }
}

void DebugLevelEditor::handle_input() {
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);
    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        // FIXME: moves everything on screen somehow
        move_entity(mouse_position);

    } else if (IsKeyPressed(KEY_F1)) {
        spawn_circular_planet(mouse_position);
    } else if (IsKeyPressed(KEY_F2)) {
        // FIXME: not being drawn at all right now
        spawn_rect_planet(mouse_position);
    } else if (IsKeyPressed(KEY_F3)) {
        spawn_rect(mouse_position);
    } else if (IsKeyPressed(KEY_F4)) {
        spawn_rect_field(mouse_position);

    } else if (IsKeyPressed(KEY_V)) {
        // debug thing for fixing bad code (lands player)
        auto& body = global->ECS->get_component<Body>(global->player_id);
        body.touching_ground = true;

    } else if (IsKeyPressed(KEY_ONE)) {
        debug_print();

    } else if (IsKeyPressed(KEY_D)) {
        // add deleter methods
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
