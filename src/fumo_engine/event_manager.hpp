#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

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
};

#endif
