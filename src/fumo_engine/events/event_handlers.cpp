#include "fumo_engine/core/fumo_engine.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

void EntityEventHandler::handle_events() {
    std::queue<Event> event_queue_copy(event_queue);
    while (!event_queue.empty()) {
        const Event& event = event_queue.front();
        handle_event(event);
        event_queue.pop();
    }
    // ... add code for going through lasting events
}

void EntityEventHandler::handle_event(const Event& event) {
    switch (event.event) {
        case EVENT_::ENTITY_JUMPED:
            FumoEvent::jumped(event);
            break;

        case EVENT_::ENTITY_MOVED:
            // handles all 4 move directions
            event.delegate_system->sys_call();
            break;

        case EVENT_::ENTITY_IDLE:
            FumoEvent::idle(event);
            break;

        case EVENT_::ENTITY_SWAPPED_ORBITS:
            FumoEvent::swapped_orbits(event);
            break;

        case EVENT_::ENTITY_COLLIDED:
            FumoEvent::collided(event);
            break;

        case EVENT_::ENTITY_DASHED:
            FumoEvent::dashed(event);
            break;
        case EVENT_::PLAYER_TRANSITIONED_SCREEN:
            FumoEvent::screen_transition(event);
            break;
    }
}

// NOTE: this function is very unoptimal, and if we REALLY
// want to check events like this often, then we should stop using a queue
// and switch to a deque or even simply a vector
bool EntityEventHandler::did_event_happen(const EVENT_& EVENT,
                                          EntityId entity_id) {
    auto copycopy(event_queue_copy);
    while (!event_queue_copy.empty()) {
        const Event& event = event_queue_copy.front();
        if (event.event == EVENT) {
            return true;
        }
        event_queue_copy.pop();
    }
    event_queue_copy = copycopy;
    return false;
}
