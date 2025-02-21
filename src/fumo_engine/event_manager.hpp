#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include <unordered_map>



struct Event {
    std::unordered_map<std::string_view, std::shared_ptr<SystemListener>>
        subscribed_system_listeners;

    virtual void event_call() = 0;
};

struct JUMP_EVENT : Event {

    // what we want to do when event happens
    void event_call() override {
        for (auto const& sys_pair : subscribed_system_listeners) {
            auto const& t_name = sys_pair.first;
            auto const& system_listener = sys_pair.second;

            system_listener->on_notify();
        }
    };
};

class EventManager {
    // class for managing all events created by systems
    // systems can:
    // - subscribe to events
    // - unsubscribe from events
    // - create events
    // - notify that events happened
    //
    // we want to pair this with ability to sleep systems and register them.
    // for ex:
    // - an event might be a trigger to start a new system altogether. maybe we even want
    // to register the system at that moment for more flexibility, since it had no reason
    // to exist in an unscheduled and unregistered state if it wasnt even really utilized
    // the whole time

    // Events are used for:
    // - starting up other systems (ex: the jump smoothing system is ran by the movement
    // system with a jump event)
    // - communication between systems
    // - decoupling of systems by passing the responsability to finish a task to another
    // one
    // - passing information. you can carry variables with the events as well

  public:
    void register_event(Event event);
    // called by a system
    void notify_event_immediate();
    void queue_event();

    template<typename T>
    void unsubscribe_event(Event event);
    template<typename T>
    void subscribe_event(Event event);
};

#endif
