#pragma once
#include <queue>

#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/event_components.hpp"

// NOTE: while this is ONLY for the player,
// this class might be generalized to any entity in the future
class EntityEventHandler {
    friend class GlobalState;

  private:
    std::queue<Event> event_queue {};
    std::queue<Event> event_queue_copy {};
    // events we want to consume in the future and only then destroy
    // std::vector<Event> lasting_event_vector {};

  public:
    inline void add_event(const Event& event) {
        event_queue.emplace(event);
    }

    bool event_happened(const EVENT_& EVENT, EntityId entity_id);

  private:
    void handle_events();
    void handle_event(const Event& event);
};

namespace EventHandler {

void jumped(const Event& event);
void idle(const Event& event);
void swapped_orbits(const Event& event);
void collided(const Event& event);
void held_key_swapping(const Event& event);

} // namespace EventHandler

struct StateHandler: System {
    Timer coyotte_timer;

    void sys_call() override {
        handle_states();
    }

    void handle_states();
    void handle_state(const EntityId& entity_id, const EntityState& entity_state);
    void end_of_frame_update();
};

struct MovedWrapper: System {
    EntityId entity_id;

    void sys_call() override {
        moved_event();
    }

    void moved_event();
};
