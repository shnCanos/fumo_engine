#include "fumo_engine/level_systems/screen_handler.hpp"

#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/screen_components.hpp"
#include "main_functions.hpp"

extern std::unique_ptr<GlobalState> global;

void ScreenTransitionHandler::check_for_screen_transition() {
    const auto& player_id = global->player_id;
    const auto& player_shape =
        global->ECS->get_component<PlayerShape>(global->player_id);
    const auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    // go through all ScreenTransitionRect
    for (const auto& entity_id : sys_entities) {

        const auto& body = global->ECS->get_component<Body>(entity_id);
        const auto& transition_rect =
            global->ECS->get_component<ScreenTransitionRect>(entity_id);

        const auto& collision = PlayerToRectCollision(player_shape,
                                                      player_body,
                                                      transition_rect.transition_rect,
                                                      body);
        if (collision.overlap) {
            // if overlap != 0, then there was a collision
            global->event_handler->add_event(
                Event {.event = EVENT_::PLAYER_TRANSITIONED_SCREEN,
                       .entity_id = player_id});

            global->ECS->replace_or_add_component(entity_id,
                                                  transition_rect.next_screen);
            // delay doing anything for the end of the frame
            // (useful for multithreading later)
        }
    }
}

namespace FumoEvent {

void screen_transition(const Event& event) {

    const auto& new_screen = global->ECS->get_component<Screen>(event.entity_id);
    auto& entity_state = global->ECS->get_component<EntityState>(event.entity_id);

    // do stuff to the camera here and all that
    // ...how tf do you do that
    UpdateCameraCenterSmoothFollow(global->camera.get(),
                                   Body {.position = new_screen.screen_position});
    // force the player to free in place until the camera is set
    // with a boolean or a disabling event
    entity_state.is_changing_screens = true;
}
} // namespace FumoEvent
