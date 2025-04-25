#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
#include "fumo_engine/core/fumo_engine.hpp"
#include "fumo_engine/level_editor/screen_handler.hpp"
#include "fumo_engine/screen_components.hpp"
#include "main_functions.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

void ScreenTransitionHandler::check_for_screen_transition() {
    // FIXME: (current) test screen transition rects
    // and associate the camera to these new rectangles

    const auto& player_id = fumo_engine->player_id;
    const auto& player_capsule =
        fumo_engine->ECS->get_component<Capsule>(fumo_engine->player_id);
    const auto& player_body =
        fumo_engine->ECS->get_component<Body>(fumo_engine->player_id);
    auto& player_state =
        fumo_engine->ECS->get_component<EntityState>(fumo_engine->player_id);

    if (Vector2Equals(
            fumo_engine->camera->target,
            fumo_engine->current_screen.screen_position.to_raylib_vec2())) {
        fumo_engine->engine_state = EngineState::GAMEPLAY_RUNNING;
    }

    // go through all ScreenTransitionRect
    for (const auto& entity_id : sys_entities) {
        const auto& body = fumo_engine->ECS->get_component<Body>(entity_id);
        const auto& transition_rect =
            fumo_engine->ECS->get_component<ScreenTransitionRect>(entity_id);

        const auto& collision =
            Collisions::CapsuleToRectCollision(player_capsule,
                                               player_body,
                                               transition_rect.transition_rect,
                                               body);
        if (collision.overlap) {
            // if overlap != 0, then there was a collision
            fumo_engine->event_handler->add_event(
                Event {.event = EVENT_::PLAYER_TRANSITIONED_SCREEN,
                       .entity_id = player_id});

            fumo_engine->ECS->replace_or_add_component(
                entity_id,
                transition_rect.next_screen);
            // delay doing anything for the end of the frame
            // (useful for multithreading later)
        }
    }
}

namespace FumoEvent {

void screen_transition(const Event& event) {

    const auto& new_screen =
        fumo_engine->ECS->get_component<Screen>(event.entity_id);
    auto& entity_state =
        fumo_engine->ECS->get_component<EntityState>(event.entity_id);

    // do stuff to the camera here and all that
    // ...how tf do you do that
    UpdateCameraCenterSmoothFollow(
        fumo_engine->camera.get(),
        Body {.position = new_screen.screen_position});
    // force the player to free in place until the camera is set
    // with a boolean or a disabling event
    // entity_state.is_changing_screens = true;
    fumo_engine->engine_state = EngineState::GAMEPLAY_PAUSED;
}
} // namespace FumoEvent
