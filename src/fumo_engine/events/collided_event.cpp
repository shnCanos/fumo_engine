#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/event_components.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

namespace FumoEvent {

void collided(const Event& event) {
    auto& player_state =
        fumo_engine->ECS->get_component<EntityState>(event.entity_id);

    CollisionEventData& collided_event =
        fumo_engine->ECS->get_component<CollisionEventData>(event.entity_id);

    // reset previous info about corner collision
    player_state.colliding_with_corner = false;
    player_state.switch_perpendicular_fix = false;

    // PRINT(collided_event.all_collisions.size())
    for (const auto& collision : collided_event.all_collisions) {
        switch (collision.collided_capsule_side) {
            case DIRECTION::DOWN:
                // PRINT_NO_NAME("WAS DOWN")
                player_state.jumping = false;
                player_state.dashes_left = 1;
                player_state.can_jump = true;
                player_state.on_ground = true;
                break;
            case DIRECTION::LEFT:
                break;
            case DIRECTION::RIGHT:
                break;
            case DIRECTION::UP:
                player_state.switch_perpendicular_fix = true;
                break;
            case DIRECTION::UP_RIGHT:
                break;
            case DIRECTION::UP_LEFT:
                break;
            case DIRECTION::DOWN_LEFT:
                break;
            case DIRECTION::DOWN_RIGHT:
                break;
            case DIRECTION::NO_DIRECTION:
                std::unreachable();
                PANIC("GAVE NO_DIRECTION TO COLLISION");
                break;
        }

        if (collision.corner_collision) {
            player_state.colliding_with_corner = true;
        }
        // here for now but if we use the other directions remove it
        // if (collision.collided_capsule_side == DIRECTION::DOWN) break;
    }
    // PRINT(player_state.colliding_with_corner)

    player_state.landed = !player_state.can_swap_orbits;

    player_state.can_swap_orbits = true;

    // remove this line
    // player_state.colliding = true;
    collided_event.all_collisions.clear();
}

} // namespace FumoEvent
