#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

// this is a macro because calling BeginMode2D is expensive and we want to avoid it
#define DRAW_OUTLINE_RECT(rect) \
    FumoVec2 TopLeft = {rect.x, rect.y}; \
    FumoVec2 TopRight = {rect.x + rect.width, rect.y}; \
    FumoVec2 BottomLeft = {rect.x, rect.y + rect.height}; \
    FumoVec2 BottomRight = {rect.x + rect.width, rect.y + rect.height}; \
    FumoDrawLineV(TopLeft, TopRight, FUMO_GREEN); \
    FumoDrawLineV(TopLeft, BottomLeft, FUMO_GREEN); \
    FumoDrawLineV(TopRight, BottomRight, FUMO_GREEN); \
    FumoDrawLineV(BottomLeft, BottomRight, FUMO_GREEN);

void ObjectRenderer::draw_planets() {
    BeginMode2D(*global->camera);

    EntityQuery circle_query {.component_mask =
                                  global->ECS->make_component_mask<Circle>(),
                              .component_filter = Filter::All};
    EntityQuery circle_grav_query {
        .component_mask =
            global->ECS->make_component_mask<CircularGravityField>(),
        .component_filter = Filter::All};
    EntityQuery rect_query {.component_mask =
                                global->ECS->make_component_mask<FumoRect>(),
                            .component_filter = Filter::All};
    EntityQuery parallel_grav_query {
        .component_mask =
            global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};
    EntityQuery outline_query {
        .component_mask = global->ECS->make_component_mask<OutlineRectFlag>(),
        .component_filter = Filter::All};
    // allow for drawing planets with and without gravity
    // could be different functions but its only one function call
    // and it would cost a lot with calling BeginMode2D many times

    for (const auto& entity_id : sys_entities) {
        const auto& body = global->ECS->get_component<Body>(entity_id);
        const auto& render = global->ECS->get_component<Render>(entity_id);

        if (global->ECS->filter(entity_id, circle_query)) {
            const auto& circle_shape =
                global->ECS->get_component<Circle>(entity_id);
            FumoDrawCircleV(body.position, circle_shape.radius, render.color);

            if (global->ECS->filter(entity_id, circle_grav_query)) {
                const auto& gravity_field =
                    global->ECS->get_component<CircularGravityField>(entity_id);
                FumoDrawCircleV(gravity_field.position,
                                gravity_field.gravity_radius,
                                FumoColorAlpha(render.color, 0.2f));
            }

        } else if (global->ECS->filter(entity_id, rect_query)) {
            const auto& rect = global->ECS->get_component<FumoRect>(entity_id);

            if (global->ECS->filter(entity_id, outline_query)) {
                DRAW_OUTLINE_RECT(rect);

            } else {
                // FumoDrawRectV(body.position,
                //               {rect.width, rect.height},
                //               render.color);
                // const auto e = Collisions::calculate_sub_rectangles(
                //     rect,
                //     body,
                //     10,
                //     global->ECS->get_component<PlayerShape>(global->player_id)
                //         .radius);
            }

            if (global->ECS->filter(entity_id, parallel_grav_query)) {
                const auto& parallel_field =
                    global->ECS->get_component<ParallelGravityField>(entity_id);

                DrawRectangle(
                    parallel_field.field_fumo_rect.x,
                    parallel_field.field_fumo_rect.y,
                    parallel_field.field_fumo_rect.width,
                    parallel_field.field_fumo_rect.height,
                    FumoColorAlpha(render.color, 0.2f).to_raylib_color());
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

        DrawRectangle(parallel_field.field_fumo_rect.x,
                      parallel_field.field_fumo_rect.y,
                      parallel_field.field_fumo_rect.width,
                      parallel_field.field_fumo_rect.height,
                      FumoColorAlpha(render.color, 0.2f).to_raylib_color());
    }
    EndMode2D();
}
