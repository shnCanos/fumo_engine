#pragma once
#include <array>
#include <cstdint>

#include "constants/constants.hpp"
#include "constants/engine_constants.hpp"
// #include <libassert/assert.hpp>
#include <queue>

// Entities are represented by unique IDs

class EntityManager {
  private:
    uint64_t living_entity_count {};
    std::queue<EntityId> available_entity_ids {};
    // Array of component_masks where the index corresponds to the entity ID
    std::array<ComponentMask, MAX_ENTITY_IDS> entity_component_masks {};

  public:
    EntityManager() {
        for (EntityId id = 0; id < MAX_ENTITY_IDS; id++) {
            available_entity_ids.push(id);
        }
    }

    EntityId create_entity() {
        living_entity_count++;
        DEBUG_ASSERT(living_entity_count < MAX_ENTITY_IDS,
                     "too many living entities",
                     living_entity_count);
        EntityId id = available_entity_ids.front();
        available_entity_ids.pop();
        return id;
    }

    void destroy_entity(EntityId entity_id) {
        DEBUG_ASSERT(entity_id < MAX_ENTITY_IDS, "exceeded MAX_ENTITY_IDS");
        // reset this entity component
        entity_component_masks[entity_id] = 0;

        // add the id again to be reused later
        available_entity_ids.push(entity_id);
        living_entity_count--;
    }

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
    // TODO: decide if we want to implement getter methods or not
    //
    // [[nodiscard]] EntityId get_entities(ComponentMask component_mask) {
    //
    // }
    // [[nodiscard]] EntityId get_entity(ComponentMask component_mask) {
    //
    // }
    // useful when we know we only have one entity or are looking for a flag struct
    // [[nodiscard]] EntityId get_flagged_entity(ComponentMask component_mask) {
    //     EntityId return_id = NO_ENTITY_FOUND;
    //     for (EntityId entity_id = 0; entity_id < living_entity_count; entity_id++) {
    //         if (has_all_components(entity_component_masks[entity_id], component_mask))
    //         {
    //             return entity_id;
    //         }
    //     }
    //     DEBUG_ASSERT(1 > 2, "no entity exists with this flag.", component_mask);
    //     return NO_ENTITY_FOUND;
    // }

    // NOTE: entity manager is *NOT* responsible for changing component_masks
    void add_to_component_mask(EntityId entity_id, ComponentId component_id) {
        DEBUG_ASSERT(living_entity_count < MAX_ENTITY_IDS,
                     "too many living entities");
        DEBUG_ASSERT(entity_id < MAX_ENTITY_IDS, "exceeded MAX_ENTITY_IDS");
        entity_component_masks[entity_id] |= 1 << component_id;
    }

    void remove_from_component_mask(EntityId entity_id,
                                    ComponentId component_id) {
        DEBUG_ASSERT(living_entity_count < MAX_ENTITY_IDS,
                     "too many living entities");
        DEBUG_ASSERT(entity_id < MAX_ENTITY_IDS, "exceeded MAX_ENTITY_IDS");
        // XOR makes it so if both bits are 1 then it turns into a 0,
        DEBUG_ASSERT(((entity_component_masks[entity_id] & component_id) != 0),
                     "component wasn't in the mask",
                     entity_component_masks[entity_id]);
        // FIXME: make sure that this works as intended
        // (test removing components later)
        entity_component_masks[entity_id] ^= 1 << component_id;
    }

    [[nodiscard]] ComponentMask get_component_mask(EntityId entity_id) {
        DEBUG_ASSERT(entity_id < MAX_ENTITY_IDS, "exceeded MAX_ENTITY_IDS");
        return entity_component_masks[entity_id];
    }

    void debug_print() {
        PRINT(living_entity_count);
        PRINT(available_entity_ids);
    }
};
