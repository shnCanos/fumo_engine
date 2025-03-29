#include "scheduler_ecs.hpp"

void SchedulerECS::debug_print_entity(EntityId entity_id) {
    auto component_mask = ecs->get_component_mask(entity_id);

    std::cerr << entity_id << " -----> ";

    for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
        // go through all components
        if (component_mask & (1 << id)) {
            std::string_view component_name = ecs->get_name_of_component_id(id);
            std::cerr << libassert::highlight_stringify(component_name) << " ";
        }
    }
}

void SchedulerECS::debug_print() {
    PRINT(all_entity_ids_debug);
    std::cerr << '\n';
    ecs->debug_print();
    std::cerr << '\n';
    debug_print_scheduler();
    std::cerr << '\n' << '\n';
}

void SchedulerECS::debug_print_scheduler() {
    PRINT(system_scheduler);
    PRINT(all_scheduled_unregistered_systems_debug);
    for (auto const& pair : all_scheduled_unregistered_systems_debug) {
        auto const& t_name = pair.first;
        auto const& system = pair.second;
        std::cerr << libassert::highlight_stringify(t_name) << " -----> "
                  << libassert::highlight_stringify(system->priority) << std::endl;
    }
    for (auto const& pair : all_scheduled_systems_debug) {
        auto const& t_name = pair.first;
        auto const& system = pair.second;
        std::cerr << libassert::highlight_stringify(t_name) << " -----> "
                  << libassert::highlight_stringify(system->priority) << std::endl;
    }
}
