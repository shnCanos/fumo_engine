#ifndef ECS_HPP 
#define ECS_HPP 
#include "component_manager.hpp"
#include "engine_constants.hpp"
#include "entity_manager.hpp"
#include "fumo_engine/component_array.hpp"
#include "fumo_engine/entity_query.hpp"
#include "system_manager.hpp"
#include <memory>
class ECS {
  private:
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<SystemManager> system_manager;

  public:
    void initialize() {
        entity_manager = std::make_unique<EntityManager>();
        component_manager = std::make_unique<ComponentManager>();
        system_manager = std::make_unique<SystemManager>();
    }
    // --------------------------------------------------------------------------------------
    // entity stuff
    [[nodiscard]] EntityId create_entity() { return entity_manager->create_entity(); }

    void destroy_entity(EntityId entity_id) {
        entity_manager->destroy_entity(entity_id);
        component_manager->notify_destroyed_entity(entity_id);
        system_manager->notify_destroyed_entity(entity_id);
    }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // component stuff
    template<typename T>
    void register_component() {
        component_manager->register_component<T>();
    }

    template<typename T>
    void entity_add_component(EntityId entity_id, T component) {
        // tell the component manager to add this entity_id associated to this component
        // this updates the entity's component mask
        component_manager->add_component<T>(entity_id, component);
        // update the entity_id's mask
        ComponentId component_id = get_component_id<T>();
        // add the component
        entity_manager->add_to_component_mask(entity_id, component_id);

        // NOTE: this is slightly slower for clarity, this method can be merged
        // so you dont have to call "get component mask" after the call
        ComponentMask component_mask = entity_manager->get_component_mask(entity_id);

        // notify the systems of the change
        // NOTE: change this so it wont notify systems until the end of the frame
        system_manager->entity_component_mask_changed(entity_id, component_mask);
    }

    template<typename T> // remove from entity
    [[nodiscard]] ComponentMask entity_remove_component(EntityId entity_id) {
        auto component_id = component_manager->remove_component<T>(entity_id);

        component_manager->get_component_id<T>();

        entity_manager->remove_from_component_mask(entity_id, component_id);

        ComponentMask component_mask = entity_manager->get_component_mask(entity_id);
        system_manager->entity_component_mask_changed(entity_id, component_mask);
        return component_mask;
        // NOTE: change this so it wont notify systems until the end of the frame
    }

    template<typename T>
    [[nodiscard]] T& get_component(EntityId entity_id) {
        return component_manager->get_component<T>(entity_id);
    }

    template<typename T>
    [[nodiscard]] ComponentId get_component_id() {
        return component_manager->get_component_id<T>();
    }
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // system stuff
    template<typename T>
    void register_system(const EntityQuery& entity_query,
                          const std::shared_ptr<T>& system_ptr) {
        system_manager->register_system<T>(entity_query, system_ptr);
    }

    template<typename T>
    void set_system_entity_query(ComponentMask component_mask) {
        system_manager->set_entity_query<T>(component_mask);
    }

    [[nodiscard]] std::shared_ptr<System> get_system(std::string_view type_name) {
        return system_manager->get_system(type_name);
    }

    // --------------------------------------------------------------------------------------
    void debug_print() {

        component_manager->debug_print();
        entity_manager->debug_print();
        system_manager->debug_print();
    }
};

#endif
