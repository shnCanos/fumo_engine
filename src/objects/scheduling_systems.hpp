#ifndef SCHEDULING_SYSTEMS_HPP
#define SCHEDULING_SYSTEMS_HPP
#include "fumo_engine/scheduler_ecs.hpp"

struct SystemInitializer : System {
    // NOTE: this is an extension of the main SchedulerECS class
    // used to awake systems when necessary, and put them to sleep

    // WARNING:
    // this is for methods commonly accessed by other systems,
    // **NOT** for normal direct access from global to register and such

    std::shared_ptr<SchedulerECS> init_ECS;

    SystemInitializer(std::shared_ptr<SchedulerECS> init_ECS) : init_ECS(init_ECS) {}

    void sys_call() override {};
    template<typename T>
    void awake_system() {
        // FIXME: finish writing these methods
    }
    template<typename T>
    void sleep_system();
        // FIXME: finish writing these methods
};
#endif
