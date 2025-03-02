#include "fumo_engine/global_state.hpp"
#include "raylib.h"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

void PlanetRenderer::draw_planet() {
    BeginMode2D(*global->camera);
    for (const auto entity_id : sys_entities) {
        const auto& body = global->ECS->get_component<Body>(entity_id);
        const auto& circle_shape = global->ECS->get_component<CircleShape>(entity_id);
        const auto& render = global->ECS->get_component<Render>(entity_id);
        const auto& gravity_field = global->ECS->get_component<GravityField>(entity_id);
        DrawCircleV(body.position, circle_shape.radius, render.color);
        DrawCircleV(body.position, circle_shape.radius + gravity_field.gravity_radius,
                    ColorAlpha(render.color, 0.2f));
    }
    EndMode2D();
}
