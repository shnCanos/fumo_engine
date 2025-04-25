#include "fumo_engine/core/global_state.hpp"
#include "renderers.hpp"

extern std::unique_ptr<GlobalState> global;

// this is a macro because calling BeginMode2D is expensive and we want to avoid it

void ObjectRenderer::draw_planets() {
    DrawFPS(10, 10);
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
            circle_shape.draw(render.color, body.position);

            if (global->ECS->filter(entity_id, circle_grav_query)) {
                const auto& gravity_field =
                    global->ECS->get_component<CircularGravityField>(entity_id);
                gravity_field.draw(render.color, body.position);
            }
            continue;
        }

        if (global->ECS->filter(entity_id, parallel_grav_query)) {
            const auto& parallel_field =
                global->ECS->get_component<ParallelGravityField>(entity_id);
            parallel_field.draw(render.color, body.position);
        }

        if (global->ECS->filter(entity_id, outline_query)) {
            const auto& rect = global->ECS->get_component<FumoRect>(entity_id);
            rect.draw_outline(render.color, body.position);
        }
        if (global->ECS->filter(entity_id, rect_query)
            && !global->ECS->filter(entity_id, outline_query)) {
            const auto& rect = global->ECS->get_component<FumoRect>(entity_id);
            rect.draw(render.color, body.position);
        }
    }

    EndMode2D();
}

// void GravFieldRenderer::draw_fields() {
//     BeginMode2D(*global->camera);
//     for (const auto& entity_id : sys_entities) {
//         const auto& render = global->ECS->get_component<Render>(entity_id);
//         const auto& parallel_field =
//             global->ECS->get_component<ParallelGravityField>(entity_id);
//
//         DrawRectangle(parallel_field.field_fumo_rect.x,
//                       parallel_field.field_fumo_rect.y,
//                       parallel_field.field_fumo_rect.width,
//                       parallel_field.field_fumo_rect.height,
//                       FumoColorAlpha(render.color, 0.2f).to_raylib_color());
//     }
//     EndMode2D();
// }
