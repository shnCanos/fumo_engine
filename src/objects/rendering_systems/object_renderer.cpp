#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

// this is a macro because calling BeginMode2D is expensive and we want to avoid it
#define DRAW_OUTLINE_RECT(rect) \
    Vector2 TopLeft = {rect.x, rect.y}; \
    Vector2 TopRight = {rect.x + rect.width, rect.y}; \
    Vector2 BottomLeft = {rect.x, rect.y + rect.height}; \
    Vector2 BottomRight = {rect.x + rect.width, rect.y + rect.height}; \
    DrawLineV(TopLeft, TopRight, GREEN); \
    DrawLineV(TopLeft, BottomLeft, GREEN); \
    DrawLineV(TopRight, BottomRight, GREEN); \
    DrawLineV(BottomLeft, BottomRight, GREEN);

void ObjectRenderer::draw_planets() {
    BeginMode2D(*global->camera);

    EntityQuery circle_query {
        .component_mask = global->ECS->make_component_mask<Circle>(),
        .component_filter = Filter::All
    };
    EntityQuery circle_grav_query {
        .component_mask = global->ECS->make_component_mask<CircularGravityField>(),
        .component_filter = Filter::All
    };
    EntityQuery rect_query {
        .component_mask = global->ECS->make_component_mask<Rectangle>(),
        .component_filter = Filter::All
    };
    EntityQuery parallel_grav_query {
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All
    };
    EntityQuery outline_query {
        .component_mask = global->ECS->make_component_mask<OutlineRectFlag>(),
        .component_filter = Filter::All
    };
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
                DrawCircleV(
                    gravity_field.position,
                    gravity_field.gravity_radius,
                    ColorAlpha(render.color, 0.2f)
                );
            }

        } else if (global->ECS->filter(entity_id, rect_query)) {
            const auto& rect = global->ECS->get_component<Rectangle>(entity_id);

            if (global->ECS->filter(entity_id, outline_query)) {
                DRAW_OUTLINE_RECT(rect);

            } else {
                DrawRectangleV(body.position, {rect.width, rect.height}, render.color);
            }

            if (global->ECS->filter(entity_id, parallel_grav_query)) {
                const auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);

                DrawRectangle(
                    parallel_field.field_rectangle.x,
                    parallel_field.field_rectangle.y,
                    parallel_field.field_rectangle.width,
                    parallel_field.field_rectangle.height,
                    ColorAlpha(render.color, 0.2f)
                );
            }
        }
    }

    EndMode2D();
}

void GravFieldRenderer::draw_fields() {
    BeginMode2D(*global->camera);
    for (const auto& entity_id : sys_entities) {
        const auto& render = global->ECS->get_component<Render>(entity_id);
        const auto& parallel_field =
            global->ECS->get_component<ParallelGravityField>(entity_id);

        DrawRectangle(
            parallel_field.field_rectangle.x,
            parallel_field.field_rectangle.y,
            parallel_field.field_rectangle.width,
            parallel_field.field_rectangle.height,
            ColorAlpha(render.color, 0.2f)
        );
    }
    EndMode2D();
}
