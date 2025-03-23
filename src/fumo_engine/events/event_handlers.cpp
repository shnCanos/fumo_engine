#include "fumo_engine/events/event_handlers.hpp"

#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

void EntityEventHandler::handle_events() {
    // also handles state and links it to events

    while (!event_queue.empty()) {
        // associate each event to the change of state we are looking for from it
        Event event = event_queue.front();
        event_queue.pop();

        handle_event(event);
    }
    // ---------------------------------------------------------------------------
    // handle state related stuff after events changed state
}

void EntityEventHandler::handle_event(const Event& event) {
    // const auto& animation_player = global->ECS->get_system<AnimationPlayer>();

    // note to self:
    // -> switch cases fall through if you do not add breaks
    // -> this means you can chain stuff inside a switch on purpose

    switch (event.event) {
        case EVENT_::ENTITY_JUMPED: {
            EventHandler::jumped(event);
            break;
        }

        case EVENT_::ENTITY_SWAPPED_ORBITS: {
            break;
        }

        case EVENT_::NO_KEY_PRESSED: {
            break;
        }

        case EVENT_::ENTITY_FELL_FROM_GROUND: {
            break;
        }
        case EVENT_::ENTITY_MOVED_LEFT:
            EventHandler::move_left(event);

        case EVENT_::ENTITY_MOVED_RIGHT:
            EventHandler::move_right(event);
            break;
    }
}
