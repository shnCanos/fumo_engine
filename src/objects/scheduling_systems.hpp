#ifndef SCHEDULING_SYSTEMS_HPP
#define SCHEDULING_SYSTEMS_HPP
#include "constants.hpp"
#include "fumo_engine/scheduler_ecs.hpp"
#include <memory>

struct SchedulerSystemECS : System {
    // NOTE: this is an extension of the main SchedulerECS class
    // used to awake systems when necessary, and put them to sleep

    // WARNING:
    // this is for methods commonly accessed by other systems,
    // **NOT** for normal direct access from global to register and such

    // weak ptr so wr get no memory leak warnings when we close the window
    // (this being shared meant that this reference wouldnt get deleted once
    // global_state variable get deleted)
    std::weak_ptr<SchedulerECS> parent_ECS;

    SchedulerSystemECS(std::shared_ptr<SchedulerECS> parent_ECS)
        : parent_ECS(parent_ECS) {}

    void sys_call() override {};
    template<typename T>
    void awake_system() {
        std::string_view t_name = libassert::type_name<T>();
        const auto& parent_ptr = parent_ECS.lock();
        const auto& system_ptr = parent_ptr->ecs->get_system(t_name);
        parent_ptr->system_scheduler.insert(system_ptr);
    }

    template<typename T>
    void sleep_system() {
        std::string_view t_name = libassert::type_name<T>();
        const auto& parent_ptr = parent_ECS.lock();
        const auto& system_ptr = parent_ptr->ecs->get_system(t_name);
        size_t erased_count = parent_ptr->system_scheduler.erase(system_ptr);
        DEBUG_ASSERT(erased_count != 0, "this system wasnt awake/scheduled.", t_name);
    }
};

// scheduling systems are systems that coordinate the starting and ending of systems
// based on things going on with the code
// for example it might run a series of systems for only 6 seconds, based on a game
// condition such as obtaining an item and playing out a cutscene for example

#endif
