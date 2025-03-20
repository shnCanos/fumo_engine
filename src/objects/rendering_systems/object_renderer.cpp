#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

void ObjectRenderer::draw_planets() {

    BeginMode2D(*global->camera);

    EntityQuery circle_query{.component_mask =
                                 global->ECS->make_component_mask<Circle>(),
                             .component_filter = Filter::All};
    EntityQuery circle_grav_query{
        .component_mask = global->ECS->make_component_mask<CircularGravityField>(),
        .component_filter = Filter::All};
    EntityQuery rect_query{.component_mask =
                               global->ECS->make_component_mask<Rectangle>(),
                           .component_filter = Filter::All};
    EntityQuery parallel_grav_query{
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};
    // allow for drawing planets with and without gravity
    // could be different functions but its only one function call
    // and it would cost a lot with calling BeginMode2D many times

    for (const auto& entity_id : sys_entities) {

        const auto& body = global->ECS->get_component<Body>(entity_id);
        const auto& render = global->ECS->get_component<Render>(entity_id);

        if (global->ECS->filter(entity_id, circle_query)) {
            const auto& circle_shape = global->ECS->get_component<Circle>(entity_id);
            DrawCircleV(body.position, circle_shape.radius, render.color);

            if (global->ECS->filter(entity_id, circle_grav_query)) {
                const auto& gravity_field =
                    global->ECS->get_component<CircularGravityField>(entity_id);
                DrawCircleV(gravity_field.position, gravity_field.gravity_radius,
                            ColorAlpha(render.color, 0.2f));
            }

        } else if (global->ECS->filter(entity_id, rect_query)) {
            const auto& rect = global->ECS->get_component<Rectangle>(entity_id);
            DrawRectangleV(body.position, {rect.width, rect.height}, render.color);

            if (global->ECS->filter(entity_id, parallel_grav_query)) {
                const auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);
                // DrawRectangleRec(parallel_field.field_rectangle,
                //                  ColorAlpha(render.color, 0.2f));

                DrawRectangleV(parallel_field.position,
                               {parallel_field.field_rectangle.width,
                                parallel_field.field_rectangle.height},
                               ColorAlpha(render.color, 0.2f));
            }
        }
        // // FIXME: DEBUG drawing
        // DrawCircleV(body.position, mouse_radius, GREEN);
    }

    EndMode2D();
}
