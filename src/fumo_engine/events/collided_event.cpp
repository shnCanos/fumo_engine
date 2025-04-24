#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/event_components.hpp"

extern std::unique_ptr<GlobalState> global;

namespace FumoEvent {

void collided(const Event& event) {
    auto& player_state =
        global->ECS->get_component<EntityState>(event.entity_id);

    CollisionEventData& collided_event =
        global->ECS->get_component<CollisionEventData>(event.entity_id);

    PRINT(collided_event.all_collisions.size())
    for (const auto& collision : collided_event.all_collisions) {
        switch (collision.collided_capsule_side) {
            case DIRECTION::DOWN:
                PRINT_NO_NAME("WAS DOWN")
                player_state.jumping = false;
                player_state.dashes_left = 1;
                player_state.can_jump = true;
                player_state.on_ground = true;
                break;
            case DIRECTION::LEFT:
            case DIRECTION::RIGHT:
            case DIRECTION::UP:
                break;
            case DIRECTION::NO_DIRECTION:
                PANIC("GAVE NO_DIRECTION TO COLLISION");
                std::unreachable();
        }
        // here for now but if we use the other directions remove it
        if (collision.collided_capsule_side == DIRECTION::DOWN) break;
    }

    player_state.landed = !player_state.can_swap_orbits;

    player_state.can_swap_orbits = true;

    // remove this line
    player_state.colliding = true;
    collided_event.all_collisions.clear();
}

} // namespace FumoEvent
