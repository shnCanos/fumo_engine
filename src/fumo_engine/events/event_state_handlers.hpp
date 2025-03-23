#ifndef EVENT_HANDLERS_HPP
#define EVENT_HANDLERS_HPP
#include <queue>

#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/flag_components.hpp"

// NOTE: while this is ONLY for the player,
// this class might be generalized to any entity in the future
class EntityEventHandler {
    friend class GlobalState;

  private:
    std::queue<Event> event_queue {};

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
void move_left(const Event& event);
void move_right(const Event& event);
void jumped(const Event& event);

} // namespace EventHandler

struct StateHandler: System {
    void sys_call() override {
        handle_states();
    }

    void handle_states();
    void handle_state(const EntityId& entity_id, const EntityState& entity_state);
    void end_of_frame_update();
};
#endif
