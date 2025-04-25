
#include "fumo_engine/core/fumo_engine.hpp"
#include "fumo_engine/core/scheduling_systems.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

void TimerHandler::update_timers() {
    auto elapsed_time = GetTime();
    const auto& scheduler_system =
        fumo_engine->ECS->get_system<SchedulerSystemECS>();

    for (const auto& timer_entity_id : sys_entities) {

        auto& timer = fumo_engine->ECS->get_component<Timer>(timer_entity_id);

        // PRINT(timer.starting_time)
        // PRINT(timer.ending_time)

        if (elapsed_time >= timer.ending_time) {
            // remove the timer associated to the system
            // and awake the system
            scheduler_system->awake_unregistered_system_from_name(
                timer.system_name);
            fumo_engine->ECS->destroy_entity(timer_entity_id);
        }
    }
}
