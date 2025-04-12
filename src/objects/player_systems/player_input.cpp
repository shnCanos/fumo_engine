#include "fumo_engine/core/global_state.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;
int key_down_event(const int& Key, const EVENT_& event, const EntityId& id);
int key_down_event_moved(const int& Key,
                         const EVENT_& event,
                         const EntityId& id,
                         const DIRECTION& direction);

void PlayerInputHandler::handle_input() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    int key_was_pressed = 0;

    key_was_pressed += key_down_event(KEY_SPACE, EVENT_::ENTITY_JUMPED, player_id);

    if (key_down_event_moved(KEY_LEFT,
                             EVENT_::ENTITY_MOVED,
                             player_id,
                             DIRECTION::LEFT)) {
        return;
    }
    if (key_down_event_moved(KEY_RIGHT,
                             EVENT_::ENTITY_MOVED,
                             player_id,
                             DIRECTION::RIGHT)) {
        return;
    }
    if (key_down_event_moved(KEY_DOWN,
                             EVENT_::ENTITY_MOVED,
                             player_id,
                             DIRECTION::DOWN)) {
        return;
    }

    if (key_down_event_moved(KEY_UP,
                             EVENT_::ENTITY_MOVED,
                             player_id,
                             DIRECTION::UP)) {
        return;
    }

    if (!key_was_pressed) {
        global->event_handler->add_event({EVENT_::ENTITY_IDLE, player_id});
    }
}

int key_down_event(const int& Key, const EVENT_& event, const EntityId& id) {
    if (IsKeyDown(Key)) {
        global->event_handler->add_event({.event = event, .entity_id = id});
        return 1;
    }
    return 0;
}

int key_down_event_moved(const int& Key,
                         const EVENT_& event,
                         const EntityId& entity_id,
                         const DIRECTION& direction) {
    if (IsKeyDown(Key)) {
        // auto add_id = [&]<typename T>(std::type_identity<T>, const auto& entity_id) {
        //     auto delegate = std::make_shared<T>();
        //     delegate->entity_id = entity_id;
        //     return delegate;
        // };
        // const auto& moved_wrapper = global->ECS->get_system<MovedWrapper>();
        // moved_wrapper->entity_id = entity_id;
        // auto& moved_event = global->ECS->get_component<MovedEventData>(entity_id);
        // moved_event.direction = direction;
        // global->event_handler->add_event({.event = event,
        //                                   .entity_id = entity_id,
        //                                   .delegate_system = moved_wrapper});
        // return 1;
        // const std::shared_ptr<MovedWrapper> moved_wrapper {};
        // moved_wrapper->entity_id = entity_id;
        auto& moved_event = global->ECS->get_component<MovedEventData>(entity_id);
        moved_event.direction = direction;

        global->event_handler->add_event(
            {.event = event,
             .entity_id = entity_id,
             .delegate_system = FumoEvent::create_delegate<MovedWrapper>(entity_id)});

        return 1;
    }

    return 0;
}
