#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

void PlanetRenderer::draw_planets() {

    BeginMode2D(*global->camera);

    EntityQuery query{.component_mask = global->ECS->make_component_mask<GravityField>(),
                      .component_filter = Filter::All};
    // allow for drawing planets with and without gravity
    // could be different functions but its only one function call
    // and it would cost a lot with calling BeginMode2D many times

    for (const auto entity_id : sys_entities) {

        const auto& body = global->ECS->get_component<Body>(entity_id);
        const auto& circle_shape = global->ECS->get_component<Circle>(entity_id);
        const auto& render = global->ECS->get_component<Render>(entity_id);
        DrawCircleV(body.position, circle_shape.radius, render.color);

        if (global->ECS->filter(entity_id, query)) {
            const auto& gravity_field =
                global->ECS->get_component<GravityField>(entity_id);
            DrawCircleV(body.position,
                        circle_shape.radius + gravity_field.gravity_radius,
                        ColorAlpha(render.color, 0.2f));
        }
    }

    EndMode2D();
}
