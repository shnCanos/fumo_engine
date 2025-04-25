#include "constants/planet_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/level_editor/level_editor.hpp"

extern std::unique_ptr<GlobalState> global;

void DebugLevelEditor::move_entity(FumoVec2 mouse_position) {
    for (const auto& entity_id : sys_entities) {
        auto& body = global->ECS->get_component<Body>(entity_id);
        float distance;
        EntityQuery circle_grav_query {
            .component_mask =
                global->ECS->make_component_mask<CircularGravityField>(),
            .component_filter = Filter::All};
        EntityQuery parallel_grav_query {
            .component_mask =
                global->ECS->make_component_mask<ParallelGravityField>(),
            .component_filter = Filter::All};
        EntityQuery only_grav_query {
            .component_mask =
                global->ECS->make_component_mask<ColliderObjectFlag>(),
            .component_filter = Filter::None};

        distance = FumoVec2Distance(body.position, mouse_position);

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
                parallel_field.field_fumo_rect.x = mouse_position.x;
                parallel_field.field_fumo_rect.y = mouse_position.y;
                return;

            } else if (global->ECS->filter(entity_id, parallel_grav_query)) {
                auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);
                parallel_field.field_fumo_rect.x = mouse_position.x;
                parallel_field.field_fumo_rect.y =
                    mouse_position.y - parallel_field.field_fumo_rect.height;
            }
            return;
        }
    }
}

void resize_rectangle(FumoRect& rectangle,
                      FumoVec2 mouse_position,
                      Body& rect_body) {
    rectangle.width = (mouse_position.x - rect_body.position.x);
    rectangle.height = (mouse_position.y - rect_body.position.y);

    if (rectangle.width < MINIMUM_OBJECT_SIZE) {
        rectangle.width = MINIMUM_OBJECT_SIZE;
    }
    if (rectangle.height < MINIMUM_OBJECT_SIZE) {
        rectangle.height = MINIMUM_OBJECT_SIZE;
    }
}

void DebugLevelEditor::drag_resizing(FumoVec2 mouse_position) {

    for (const auto& entity_id : sys_entities) {

        auto& body = global->ECS->get_component<Body>(entity_id);

        EntityQuery parallel_grav_query {
            .component_mask =
                global->ECS->make_component_mask<ParallelGravityField>(),
            .component_filter = Filter::All};

        if (global->ECS->filter(entity_id, parallel_grav_query)) {
            auto& parallel_field =
                global->ECS->get_component<ParallelGravityField>(entity_id);
            //
            BeginMode2D(*global->camera);
            // Rectangle gaming {
            //     .x = body.position.x + parallel_field.field_fumo_rect.width,
            //     .y = body.position.y - parallel_field.field_fumo_rect.height,
            //     .width = 100,
            //     .height = 100};
            // DrawRectangleRec(gaming, GOLD);

            Rectangle gaming2 {.x = body.position.x,
                               .y = body.position.y,
                               .width = parallel_field.field_fumo_rect.width
                                   + MINIMUM_OBJECT_SIZE,
                               .height = parallel_field.field_fumo_rect.height
                                   + MINIMUM_OBJECT_SIZE};

            DrawRectangleRec(gaming2, GREEN);
            EndMode2D();

            if (CheckCollisionPointRec(mouse_position.to_raylib_vec2(),
                                       gaming2)) {
                resize_rectangle(parallel_field.field_fumo_rect,
                                 mouse_position,
                                 body);
                return;
            }
        }
    }
}

// mouse_position = {.x = mouse_position.x - default_rect_width / 2.0f,
//                   .y = mouse_position.y - default_rect_height / 2.0f};
