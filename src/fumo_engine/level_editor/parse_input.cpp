#include "fumo_engine/core/fumo_engine.hpp"
#include "fumo_engine/level_editor/level_editor.hpp"
#include "fumo_engine/serialization/fumo_serializer.hpp"
extern std::unique_ptr<FumoEngine> fumo_engine;

void DebugLevelEditor::handle_input() {
    fumo_engine->camera->zoom += ((float)GetMouseWheelMove() * 0.05f);
    FumoVec2 mouse_position = to_fumo_vec2(
        GetScreenToWorld2D(GetMousePosition(), *fumo_engine->camera));
    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (IsKeyDown(KEY_R)) {
            drag_resizing(mouse_position);
        } else {
            move_entity(mouse_position);
        }
    }
    if (IsKeyPressed(KEY_P)) {
        reset_position();
    }
    if (IsKeyPressed(KEY_D)) {
        delete_planet(mouse_position);
    }
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        if (IsKeyPressed(KEY_S)) {
            save_level();
        }
    }
    if (IsKeyPressed(KEY_F1)) {
        spawn_circular_planet(mouse_position);
    }
    if (IsKeyPressed(KEY_F2)) {
        spawn_rect(mouse_position);
    }
    if (IsKeyPressed(KEY_F3)) {
        spawn_rect_field(mouse_position);
    }
    if (IsKeyPressed(KEY_F4)) {
        spawn_transition_rect(mouse_position);
    }
}

//---------------------------------------------------------
// generic functionality

void DebugLevelEditor::save_level() {
    const auto& level_serializer =
        fumo_engine->ECS->get_system<LevelSerializer>();
    // level_serializer->deserialize_levels();
    level_serializer->serialize_levels();
}

void DebugLevelEditor::reset_position() {
    auto& body = fumo_engine->ECS->get_component<Body>(fumo_engine->player_id);
    body.position = screenCenter;
}
