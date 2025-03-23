#include "fumo_engine/core/global_state.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;
int key_down_event(int Key, EVENT_ event, EntityId id);

void PlayerInputHandler::handle_input() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    int key_was_pressed = 0;

    key_was_pressed += key_down_event(KEY_SPACE, EVENT_::ENTITY_JUMPED, player_id);
    key_was_pressed += key_down_event(KEY_LEFT, EVENT_::ENTITY_MOVED_LEFT, player_id);
    key_was_pressed += key_down_event(KEY_RIGHT, EVENT_::ENTITY_MOVED_RIGHT, player_id);
    key_was_pressed += key_down_event(KEY_DOWN, EVENT_::ENTITY_MOVED_DOWN, player_id);
    key_was_pressed += key_down_event(KEY_UP, EVENT_::ENTITY_MOVED_UP, player_id);

    if (!key_was_pressed) {
        global->event_handler->add_event({EVENT_::ENTITY_IDLE, player_id});
    }
}

int key_down_event(int Key, EVENT_ event, EntityId id) {
    if (IsKeyDown(Key)) {
        global->event_handler->add_event(
            {.event = event, .entity_id = id}
        );
        return 1;
    }
    return 0;
}
