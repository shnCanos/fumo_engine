// clang-format off
#include "../constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "systems.hpp"
#include "objects/factory_systems.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
// clang-format on

extern std::unique_ptr<GlobalState> global;

void InputHandlerLevelEditor::handle_input() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        move_planet();
    } else if (IsKeyPressed(KEY_S)) {
        spawn_planet();
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_D)) {
            delete_all_created_planets();
        } else if (IsKeyPressed(KEY_R)) {
            resize_planet(0.6666f);
        }
    } else if (IsKeyPressed(KEY_D)) {
        delete_created_planet();
    } else if (IsKeyPressed(KEY_R)) {
        resize_planet(1.5f);
    } else if (IsKeyPressed(KEY_ONE)) {
        debug_print();
    }
}

void InputHandlerLevelEditor::debug_print() {
    auto debugger_ptr = global->ECS->get_system<Debugger>();
    debugger_ptr->global_debug();
}
void InputHandlerLevelEditor::spawn_planet() {
    Vector2 mouse_position = GetMousePosition();
    DrawCircleLinesV(mouse_position, mouse_radius, GREEN);
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    planet_factory_ptr->create_default_planet(mouse_position);
}

void InputHandlerLevelEditor::resize_planet(float resize) {
    Vector2 mouse_position = GetMousePosition();
    DrawCircleLinesV(mouse_position, mouse_radius, GREEN);
    for (auto entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        float distance = Vector2Distance(mouse_position, body.position);

        if (mouse_radius + circle_shape.radius > distance) {
            circle_shape.radius *= resize;
            return;
        }
    }
}

void InputHandlerLevelEditor::delete_created_planet() {
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    Vector2 mouse_position = GetMousePosition();
    DrawCircleLinesV(mouse_position, mouse_radius, GREEN);
    for (auto entity_id : planet_factory_ptr->sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        float distance = Vector2Distance(mouse_position, body.position);
        if (mouse_radius + circle_shape.radius > distance) {
            planet_factory_ptr->delete_planet(entity_id);
            return;
        }
    }
}

void InputHandlerLevelEditor::delete_all_created_planets() {
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    planet_factory_ptr->delete_all_planets();
}

void InputHandlerLevelEditor::move_planet() {
    Vector2 mouse_position = GetMousePosition();
    DrawCircleLinesV(mouse_position, mouse_radius, GREEN);
    for (auto entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        float distance = Vector2Distance(mouse_position, body.position);
        if (mouse_radius + circle_shape.radius > distance) {
            auto body_movement_ptr = global->ECS->get_system<BodyMovement>();
            body_movement_ptr->move_towards_position(body, GetMousePosition());
            // FIXME: think if its okay to update position here
            body.position += body.velocity * global->frametime;
            return;
        }
    }
}

// void InputHandlerLevelEditor::delete_planet() {
//     Vector2 mouse_position = GetMousePosition();
//     DrawCircleLinesV(mouse_position, mouse_radius, GREEN);
//     for (auto entity_id : sys_entities) {
//         auto& body = global->ECS->get_component<Body>(entity_id);
//         auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
//         float distance = Vector2Distance(mouse_position, body.position);
//         if (mouse_radius + circle_shape.radius > distance) {
//             global->ECS->destroy_entity(entity_id);
//             return;
//         }
//     }
// }
// void InputHandlerLevelEditor::delete_all_planets() {
//     // TODO: i dont like the way this is done
//     // i dont think this system should own the planet ids
//     for (auto entity_id : sys_entities) {
//         global->ECS->destroy_entity(entity_id);
//     }
// }
