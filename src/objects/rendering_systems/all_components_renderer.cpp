#include "component_renderer.hpp"
#include "fumo_engine/core/entity_query.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/screen_components.hpp"
#include "main_functions.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;
// required to access the queries
#define XMACRO(Type) extern EntityQuery Type##_query;
ALL_COMPONENTS_X_MACRO()
#undef XMACRO

#define DRAW_ALL_COMPONENTS_X_MACRO() \
    XMACRO(Circle) \
    XMACRO(FumoRect) \
    XMACRO(Capsule) \
    XMACRO(ParallelGravityField) \
    XMACRO(CircularGravityField) \
    XMACRO(OutlineRect) \
    XMACRO(ScreenTransitionRect) \
    XMACRO(Line)

void ComponentRenderer::draw_all_components() {
    DrawFPS(10, 10);
    BeginMode2D(*fumo_engine->camera);

#define XMACRO(Type) \
    if (fumo_engine->ECS->filter(entity_id, Type##_query)) { \
        const auto& shape = fumo_engine->ECS->get_component<Type>(entity_id); \
        shape.draw(render.color, body.position); \
        continue; \
    }

    for (const auto& entity_id : sys_entities) {
        const auto& body = fumo_engine->ECS->get_component<Body>(entity_id);
        const auto& render = fumo_engine->ECS->get_component<Render>(entity_id);

        DRAW_ALL_COMPONENTS_X_MACRO()
    }

#undef XMACRO // dont forget to undefine the xmacro

    EndMode2D();
}
