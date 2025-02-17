#ifndef SCHEDULER_ECS_HPP
#define SCHEDULER_ECS_HPP
#include "ECS.hpp"
#include "constants.hpp"
#include "entity_query.hpp"
#include "fumo_engine/component_array.hpp"
#include "fumo_engine/engine_constants.hpp"
#include <memory>
//
//--------------------------------------------------------------------------------------
//

using Priority = uint64_t;

// class Scheduler {
//   public:

// Array of component_masks where the index corresponds to the entity ID
// used to delay updates to the end of each frame
// std::set<EntityId> entities_to_create{};
// std::set<EntityId> entities_to_destroy{};
// each entityId corresponds to an index on this array
// std::set<EntityId> entities_to_update_components{};
// std::array<ComponentMask, MAX_ENTITY_IDS> scheduled_entity_component_masks{};
// NOTE: i have concluded that there isnt a real need to schedule these changes
// to the end of the frame. i can still do this, but i feel it adds little benefit
// as we might want to create systems that depend on some of these changes
// plus its not even necessarily more efficient as shared state is still required
// };

class SchedulerECS {

    // only missing separating systems from the entities and components
    // and running all the systems at the end
    // but i think as long as i dont touch the systems, they dont communicate with EC
    // part so it should be trivials

  private:
    std::array<std::shared_ptr<System>, MAX_SYSTEMS> system_scheduler{};
    std::array<std::shared_ptr<System>, MAX_SYSTEMS> unscheduled_systems{};
    std::unique_ptr<ECS> ecs;
    Priority current_max_priority{};

  public:
    std::array<EntityId, MAX_ENTITY_IDS> all_entity_ids_debug{};
    void initialize() {
        ecs = std::make_unique<ECS>();
        ecs->initialize();
    }
    // --------------------------------------------------------------------------------------
    // entity stuff
    [[nodiscard]] EntityId create_entity() {
        // added "scheduled" to allude to the fact this action only happens by
        // the end of the frame, can remove later
        EntityId entity_id = ecs->create_entity();
        // scheduler.entities_to_create.insert(entity_id);
        all_entity_ids_debug[entity_id] = entity_id;
        return entity_id;
    };

    void destroy_entity(EntityId entity_id) {
        // schedule deletion for the end of the frame
        // scheduler.entities_to_destroy.insert(entity_id);
        ecs->destroy_entity(entity_id);
        all_entity_ids_debug[entity_id] = NO_ENTITY_FOUND;
    }
    // get all matching entities for these types
    // template<typename ...Types>
    // [[nodiscard]] EntityId get_entities() {
    //     ComponentMask component_mask = make_component_mask<Types...>();
    // }
    // template<typename ...Types>
    // [[nodiscard]] EntityId get_entity() {
    //     ComponentMask component_mask = make_component_mask<Types...>();
    // }
    // // useful when we know we only have one entity or are looking for a flag struct
    // template<typename T>
    // [[nodiscard]] EntityId get_flagged_entity() {
    //
    //     ComponentMask component_mask = 0;
    //     component_mask |= get_component_id<T>();
    // }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // component stuff
    template<typename T>
    void register_component() {
        ecs->register_component<T>();
    }
    template<typename T>
    void entity_add_component(EntityId entity_id, T component) {
        // scheduler.scheduled_entity_component_masks[entity_id] =

        ecs->entity_add_component(entity_id, component);
    }
    template<typename T> // remove from entity
    void entity_remove_component(EntityId entity_id) {
        // scheduler.scheduled_entity_component_masks[entity_id] =
        ecs->entity_remove_component<T>(entity_id);
    }
    template<typename T>
    [[nodiscard]] T& get_component(EntityId entity_id) {
        return ecs->get_component<T>(entity_id);
    }
    template<typename T>
    [[nodiscard]] ComponentId get_component_id() {
        return ecs->get_component_id<T>();
    }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // system stuff
    template<typename T, Priority priority, typename... Types>
    void register_system(EntityQuery entity_query, Types&... args) {

        DEBUG_ASSERT(system_scheduler[priority] == nullptr, "already used this priority.",
                     system_scheduler);

        std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);
        ecs->register_system<T>(entity_query, system_ptr);

        system_scheduler[priority] = system_ptr;
        current_max_priority++;
        // return system_ptr;
    }

    // WARNING: this system wont be called in run_systems() call
    template<typename T, typename... Types>
    void register_system_unscheduled(EntityQuery entity_query, Types&... args) {
        std::shared_ptr<T> system_ptr = std::make_shared<T>(args...);
        ecs->register_system<T>(entity_query, system_ptr);
        // return system_ptr;
    }

    void run_systems() {
        for (Priority priority = 0; priority < current_max_priority; priority++) {

            std::shared_ptr<System> system_ptr = system_scheduler[priority];
            DEBUG_ASSERT(system_ptr != nullptr, "skipped a priority value.", priority,
                         current_max_priority, system_scheduler);
            system_ptr->sys_call();
        }
    }
    // returns the system **cast** from the System interface
    template<typename T>
    [[nodiscard]] std::shared_ptr<T> get_system() {
        std::string_view type_name = libassert::type_name<T>();
        return std::static_pointer_cast<T>(ecs->get_system(type_name));
    }
    template<typename... Types>
    [[nodiscard]] ComponentMask make_component_mask() {
        ComponentMask component_mask = 0;
        (add_id_to_component_mask<Types>(component_mask), ...);
        return component_mask;
    }

    void debug_print() {
        PRINT(all_entity_ids_debug);
        ecs->debug_print();
    }

  private:
    template<typename T>
    void add_id_to_component_mask(ComponentMask& component_mask) {

        auto var = std::is_base_of_v<System, T>;
        DEBUG_ASSERT(var != true,
                     "cant add a system to another system's component mask.");

        component_mask |= 1 << get_component_id<T>();
    }
    // --------------------------------------------------------------------------------------
};

#endif
