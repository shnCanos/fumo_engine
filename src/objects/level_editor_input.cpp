// clang-format off
#include "../constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "fumo_engine/scheduling_systems.hpp"
#include "objects/components.hpp"
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
    } else if (IsKeyPressed(KEY_Q)) {
        spawn_planet_no_gravity();
    } else if (IsKeyPressed(KEY_A)) {
        spawn_aggregate_field_planet();
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_D)) {
            delete_all_created_planets();
        } else if (IsKeyPressed(KEY_R)) {
            resize_planet(0.80f);
        }
    } else if (IsKeyPressed(KEY_D)) {
        delete_created_planet();
    } else if (IsKeyPressed(KEY_R)) {
        resize_planet(1.25f);
    } else if (IsKeyPressed(KEY_ONE)) {
        debug_print();
    } else if (IsKeyPressed(KEY_V)) {
        auto& body = global->ECS->get_component<Body>(global->player_id);
        body.touching_ground = true;
    }
    // } else if (IsKeyPressed(KEY_THREE)) {
    //     const auto& scheduler_ptr = global->ECS->get_system<SchedulerSystemECS>();
    //     scheduler_ptr->sleep_system<PlanetRenderer>();
    // }
}

void InputHandlerLevelEditor::debug_print() {
    auto debugger_ptr = global->ECS->get_system<Debugger>();
    debugger_ptr->global_debug();
}
void InputHandlerLevelEditor::spawn_planet() {
    const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
    scheduler_system->sleep_unregistered_system_for<InputHandlerLevelEditor>(0.3f);
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);

    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    // FIXME: remove the hardcoded camera relative spawning
    planet_factory_ptr->create_default_planet(mouse_position, BLUE);
}

void InputHandlerLevelEditor::spawn_aggregate_field_planet() {
    const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
    scheduler_system->sleep_unregistered_system_for<InputHandlerLevelEditor>(0.3f);
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);

    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    // FIXME: remove the hardcoded camera relative spawning
    planet_factory_ptr->create_default_aggregate_field_planet(mouse_position, GREEN);
}

void InputHandlerLevelEditor::spawn_planet_no_gravity() {
    const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
    scheduler_system->sleep_unregistered_system_for<InputHandlerLevelEditor>(0.3f);
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);

    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);

    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    planet_factory_ptr->create_planet_no_gravity(mouse_position, YELLOW);
}

void InputHandlerLevelEditor::resize_planet(float resize) {
    const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
    scheduler_system->sleep_unregistered_system_for<InputHandlerLevelEditor>(0.3f);
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);
    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);

    EntityQuery query{.component_mask = global->ECS->make_component_mask<GravityField>(),
                      .component_filter = Filter::All};

    for (auto entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        float distance = Vector2Distance(mouse_position, body.position);

        if (mouse_radius + circle_shape.radius > distance) {
            circle_shape.radius *= resize;

            if (global->ECS->filter(entity_id, query)) {
                auto& gravity_field =
                    global->ECS->get_component<GravityField>(entity_id);
                gravity_field.gravity_radius *= resize * resize * resize;
            }

            return;
        }
    }
}

void InputHandlerLevelEditor::delete_created_planet() {
    auto planet_factory_ptr = global->ECS->get_system<PlanetFactory>();
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);
    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);
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
    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), *global->camera);
    DrawCircleLinesV(GetMousePosition(), mouse_radius, GREEN);
    for (auto entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        float distance = Vector2Distance(mouse_position, body.position);
        if (mouse_radius + circle_shape.radius > distance) {
            // auto body_movement_ptr = global->ECS->get_system<BodyMovement>();
            // body_movement_ptr->move_towards_position(body, mouse_position);
            // // FIXME: think if its okay to update position here
            // body.position += body.velocity * global->frametime;

            body.position = mouse_position;
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
