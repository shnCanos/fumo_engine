#ifndef EVENT_HANDLERS_HPP
#define EVENT_HANDLERS_HPP
#include <queue>

#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/event_components.hpp"
#include "objects/generic_systems/systems.hpp"

// NOTE: while this is ONLY for the player,
// this class might be generalized to any entity in the future
class EntityEventHandler {
    friend class GlobalState;

  private:
    std::queue<Event> event_queue {};
    // events we want to consume in the future and only then destroy
    std::vector<Event> lasting_event_vector {};

  public:
    inline void add_event(const Event& event) {
        event_queue.emplace(event);
    }

  private:
    void handle_events();
    void handle_event(const Event& event);
    //--------------------------------------------------------------------------------------
    // event handling
    //--------------------------------------------------------------------------------------
};

namespace EventHandler {

void jumped(const Event& event);
void idle(const Event& event);
void swapped_orbits(const Event& event);
void collided(const Event& event);

} // namespace EventHandler

struct StateHandler: System {
    void sys_call() override {
        handle_states();
    }

    void handle_states();
    void handle_state(const EntityId& entity_id, const EntityState& entity_state);
    void end_of_frame_update();
};

struct MovedWrapper: System {
    DIRECTION direction;
    EntityId entity_id;
    DIRECTION previous_direction = DIRECTION::NO_DIRECTION;
    DIRECTION continue_in_direction;

    void sys_call() override {
        moved_event();
    }

    void moved_event();
};
#endif
