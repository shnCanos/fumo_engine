#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP
#include "constants.hpp"
#include "fumo_engine/entity_query.hpp"
#include "system_base.hpp"
#include <libassert/assert.hpp>
#include <string_view>
#include <unordered_map>

class SystemManager {
  private:
    // convert t_name -> respective component mask
    // simple map implementation, no need for premature optimization
    std::unordered_map<std::string_view, EntityQuery> system_entity_queries;
    // t_name -> respective System pointer
    std::unordered_map<std::string_view, std::shared_ptr<System>> all_systems;

    std::unordered_map<std::string_view, std::shared_ptr<System>> unregistered_systems;

  public:
    template<typename T>
    void register_system(const EntityQuery& entity_query,
                         const std::shared_ptr<System>& system_ptr) {
        std::string_view t_name = libassert::type_name<T>();

        DEBUG_ASSERT(!all_systems.contains(t_name), "registered the system twice.",
                     all_systems);
        all_systems.insert({t_name, system_ptr});

        set_entity_query<T>(entity_query);
    }
    template<typename T>
    void add_unregistered_system(const std::shared_ptr<System>& system_ptr) {
        std::string_view t_name = libassert::type_name<T>();

        DEBUG_ASSERT(!unregistered_systems.contains(t_name),
                     "added the unregistered system twice.", unregistered_systems);

        unregistered_systems.insert({t_name, system_ptr});
    }

  private:
    template<typename T>
    void set_entity_query(const EntityQuery& entity_query) {
        std::string_view t_name = libassert::type_name<T>();
        DEBUG_ASSERT(all_systems.contains(t_name),
                     "tried using system without registering!", all_systems);
        system_entity_queries.insert({t_name, entity_query});
    }

  public:
    // TODO: maybe remove these extra public private later

    [[nodiscard]] std::shared_ptr<System> get_system(std::string_view t_name) {

        DEBUG_ASSERT(all_systems.contains(t_name) ||
                         unregistered_systems.contains(t_name),
                     "this system hasn't been added or registered.", all_systems,
                     unregistered_systems);

        if (unregistered_systems.contains(t_name)) {
            return unregistered_systems[t_name];
        }
        return all_systems[t_name];
    }
    // template<typename T> // will i use this? not sure (consider deleting)
    // void change_component_mask(ComponentMask component_mask) {
    //     std::string_view t_name = typeid(T).name();
    //     DEBUG_ASSERT(!all_systems.contains(t_name), "tried using system without
    //     registering!",
    //                  all_systems);
    //     system_component_masks[t_name] = component_mask;
    // }
    void notify_destroyed_entity(EntityId entity_id) {
        // erases destroyed entity from all sets
        // std::set requires no checks
        for (auto const& pair : all_systems) {
            auto const& system = pair.second;
            // notice we can erase even if the element wasnt there
            // (simplifying the implementation so i dont go insane)
            system->sys_entities.erase(entity_id);
        }
    }
    void entity_component_mask_changed(EntityId entity_id, ComponentMask component_mask) {
        // notify all systems that the entity has changed
        // and if matching, add this entity to the system
        for (auto const& pair : all_systems) {
            auto const& t_name = pair.first;
            auto const& system = pair.second;
            auto& entity_query = system_entity_queries[t_name];
            if (entity_query.filter(component_mask)) {
                system->sys_entities.insert(entity_id);
            } else {
                // test if this works later
                system->sys_entities.erase(entity_id);
            }
        }
    }
    void debug_print() {
        PRINT(all_systems);
        PRINT(unregistered_systems);
    }
};
#endif
