
#include "fumo_engine/global_state.hpp"
#include "fumo_engine/scheduling_systems.hpp"

extern std::unique_ptr<GlobalState> global;

void TimerHandler::update_timers() {
    auto elapsed_time = GetTime();
    const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();

    for (const auto& timer_entity_id : sys_entities) {

        auto& timer = global->ECS->get_component<Timer>(timer_entity_id);

        // PRINT(timer.starting_time)
        // PRINT(timer.ending_time)


        if (elapsed_time >= timer.ending_time) {
            // remove the timer associated to the system
            // and awake the system
            scheduler_system->awake_system_from_name(timer.system_name);
            global->ECS->destroy_entity(timer_entity_id);
        }
    }
}
