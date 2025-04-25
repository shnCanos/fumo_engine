#pragma once
#include <queue>

#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/event_components.hpp"

// NOTE: while this is ONLY for the player,
// this class might be generalized to any entity in the future
class EntityEventHandler {
    friend class FumoEngine;

  private:
    std::queue<Event> event_queue {};
    std::queue<Event> event_queue_copy {};
    // events we want to consume in the future and only then destroy
    std::vector<Event> lasting_event_vector {};

  public:
    inline void add_event(const Event& event) { event_queue.push(event); }

    bool did_event_happen(const EVENT_& EVENT, EntityId entity_id);

  private:
    void handle_events();
    void handle_event(const Event& event);
};

namespace FumoEvent {

void jumped(const Event& event);
void idle(const Event& event);
void swapped_orbits(const Event& event);
void collided(const Event& event);
void dashed(const Event& event);
void held_key_swapping(const Event& event);
void screen_transition(const Event& event);

template<typename T>
std::shared_ptr<T> create_delegate(EntityId entity_id) {
    auto var = std::is_base_of_v<System, T>;

    DEBUG_ASSERT(var,
                 "Can only add Systems as delegates.",
                 libassert::type_name<T>());

    std::shared_ptr<T> delegate = std::make_shared<T>();
    delegate->entity_id = entity_id;
    return delegate;
}

} // namespace FumoEvent

struct StateHandler: System {
    // Timer coyotte_timer;

    void sys_call() override { handle_states(); }

    void handle_states();
    void handle_state(const EntityId& entity_id,
                      const EntityState& entity_state);
    void end_of_frame_update();
    void jump_and_gravity_state_handler(Body& player_body,
                                        Capsule& player_capsule,
                                        AnimationInfo& player_animation,
                                        EntityState& player_state);

    void dash_state_handler(Body& player_body,
                            Capsule& player_capsule,
                            AnimationInfo& player_animation,
                            EntityState& player_state);
    void movement_state_handler(Body& player_body,
                                Capsule& player_shape,
                                EntityState& player_state);
    // void screen_transition_handler(EntityState& player_state);
};

struct MovedWrapper: System {
    EntityId entity_id;

    void sys_call() override { moved_event(); }

    void moved_event();
};
