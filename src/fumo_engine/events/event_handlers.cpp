#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"

extern std::unique_ptr<GlobalState> global;

void EntityEventHandler::handle_events() {
    while (!event_queue.empty()) {
        const Event& event = event_queue.front();
        handle_event(event);
        event_queue.pop();
    }
}

void EntityEventHandler::handle_event(const Event& event) {
    switch (event.event) {
        case EVENT_::ENTITY_JUMPED:
            // only pop when we are allowed to jump again
            EventHandler::jumped(event);
            break;

        case EVENT_::ENTITY_MOVED:
            // handles all 4 move directions
            event.delegate_system->sys_call();
            break;

        case EVENT_::ENTITY_IDLE:
            EventHandler::idle(event);
            break;

        case EVENT_::ENTITY_SWAPPED_ORBITS:
            EventHandler::swapped_orbits(event);
            break;

        case EVENT_::ENTITY_COLLIDED:
            EventHandler::collided(event);
            break;
        // case EVENT_::ENTITY_FELL_FROM_GROUND:
        //     break;
    }
}
