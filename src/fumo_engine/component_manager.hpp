#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP
#include "component_array.hpp"
#include "constants.hpp"
#include "engine_constants.hpp"
#include <iostream>
#include <memory>
#include "system_base.hpp"
#endif

class ComponentManager {

  private:
    // each type needs to map to a respective id
    // NOTE: (components are never unregistered in this implementation)
    std::unordered_map<std::string_view, ComponentId> component_ids;
    std::unordered_map<std::string_view, std::shared_ptr<IComponentArray>>
        component_arrays;
    ComponentId current_component_id{};

  public:
    template<typename T>
    void register_component() {
        std::string_view t_name = libassert::type_name<T>();
        DEBUG_ASSERT(!component_ids.contains(t_name), "already added this component",
                     component_ids);
        // assign the name a unique id (used for the component bitmasks)
        component_ids.insert({t_name, current_component_id});

        // use the t_name also for creating a new array of this component
        component_arrays.insert({t_name, std::make_shared<ComponentArray<T>>()});

        current_component_id++;
    }
    template<typename T>
    [[nodiscard]] ComponentId get_component_id() {
        std::string_view t_name = libassert::type_name<T>();
        DEBUG_ASSERT(component_ids.contains(t_name), "forgot to register component",
                     component_ids, current_component_id);
        // we use this component id to make component bitmasks
        return component_ids[t_name];
    }
    template<typename T>
    T& get_component(EntityId entity_id) {
        std::string_view t_name = libassert::type_name<T>();
        DEBUG_ASSERT(component_ids.contains(t_name), "forgot to register component",
                     component_ids, t_name);
        std::shared_ptr<ComponentArray<T>> cast_component_array =
            std::static_pointer_cast<ComponentArray<T>>(component_arrays[t_name]);
        return cast_component_array->get_component_data(entity_id);
    }

    // responible for changing the component mask of the entity
    template<typename T>
    void add_component(EntityId entity_id, T component) {
        std::string_view t_name = libassert::type_name<T>();
        auto var = std::is_base_of_v<System, T>;

        DEBUG_ASSERT(var != true, "cant register a system as a component.", t_name,
                     component);

        DEBUG_ASSERT(component_ids.contains(t_name), "forgot to register component",
                     component_ids);
        // we now cast away from our interface into our **known** type conversion
        // much better than when i was using dynamic_cast<> to find out what the type of
        // the interface was
        std::shared_ptr<ComponentArray<T>> cast_component_array =
            std::static_pointer_cast<ComponentArray<T>>(component_arrays[t_name]);

        cast_component_array->add_component_data(entity_id, component);
    }
    // responsible for changing the component mask of the entity
    template<typename T>
    void remove_component(EntityId entity_id) {

        std::string_view t_name = libassert::type_name<T>();
        DEBUG_ASSERT(component_ids.contains(t_name), "forgot to register component",
                     component_ids);
        ComponentArray<T> cast_component_array =
            std::static_pointer_cast<ComponentArray<T>>(component_arrays[t_name]);
        cast_component_array->remove_component_data(entity_id);
    }

    void notify_destroyed_entity(EntityId entity_id) {
        // erases destroyed entity from all sets
        for (auto const& pair : component_arrays) {
            auto const& component_array = pair.second;
            // notice we can erase even if the element wasnt there
            // (simplifying the implementation so i dont go insane)
            component_array->entity_destroyed(entity_id);
        }
    }
    void debug_print() {
        PRINT(component_arrays);
    }
};
