#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "raylib.h"
#include "systems.hpp"

extern std::unique_ptr<GlobalState> global;

void PlanetRenderer::draw_planet() {
    for (const auto entity_id : sys_entities) {
        const auto& body = global->ECS.get_component<Body>(entity_id);
        const auto& circle_shape = global->ECS.get_component<CircleShape>(entity_id);
        const auto& render = global->ECS.get_component<Render>(entity_id);
        DrawCircleV(body.position, circle_shape.radius, render.color);
    }
}
