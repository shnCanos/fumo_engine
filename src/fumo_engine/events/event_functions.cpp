
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"
#include "objects/generic_systems/systems.hpp"

extern std::unique_ptr<GlobalState> global;

namespace EventHandler {

void jumped(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);
    if (!player_state.can_jump) {
        return;
    }

    if (player_state.jumping) {
        // cancel previous jump if we were in the middle of it
        player_body.iterations = 0;
    }
    if (player_state.on_ground) {
        player_state.jumping = true;
        player_state.on_ground = false;
        player_state.can_jump = false;
        BodyMovement::jump(player_body, event.entity_id);
    }
}

void idle(const Event& event) {
    auto& player_animation = global->ECS->get_component<AnimationInfo>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.on_ground) {
        AnimationPlayer::play(player_animation, "idle");
    }

    // reset moved_wrapper so we can allow for smoother controls,
    // whilc also not breaking the direction of the movement when we stop
    const auto& moved_wrapper = global->ECS->get_system<MovedWrapper>();
    moved_wrapper->previous_direction = DIRECTION::NO_DIRECTION;
}

void swapped_orbits(const Event& event) {
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    player_state.can_swap_orbits = false;
}

void collided(const Event& event) {
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    player_state.on_ground = true;
    player_state.can_swap_orbits = true;
    player_state.can_jump = true;
}

} // namespace EventHandler

// want this behavior:
// > when the player presses a direction, we check what that direction
// > is relative to the gravity direction coordinates
// > then we solve as follows:
// -> we take inputs literally, as the player requesting to move towards a direction.
// -> then we check if that is possible based on rotation angle
// -> we either accept, or reject the input

enum struct ACCEPT { HORIZONTAL, VERTICAL, HORIZONTAL_INVERT, VERTICAL_INVERT };

DIRECTION opposite_direction(DIRECTION direction) {
    if (direction == DIRECTION::LEFT) {
        return DIRECTION::RIGHT;
    }
    if (direction == DIRECTION::RIGHT) {
        return DIRECTION::LEFT;
    }
    if (direction == DIRECTION::UP) {
        return DIRECTION::DOWN;
    }
    if (direction == DIRECTION::DOWN) {
        return DIRECTION::UP;
    }
    PANIC("UNREACHABLE OR GAVE PREVIOUS_DIRECTION (FIX THIS)");
    std::unreachable();
}

ACCEPT find_acceptance_state(float rotation) {
    PRINT(rotation)
    if (-135 <= rotation && rotation <= -45) {
        return ACCEPT::VERTICAL_INVERT;
    }
    if (45 <= rotation && rotation <= 135) {
        return ACCEPT::VERTICAL;
    }
    if (-45 <= rotation && rotation <= 45) {
        return ACCEPT::HORIZONTAL;
    }
    if (135 <= rotation || rotation <= -135) {
        return ACCEPT::HORIZONTAL_INVERT;
    }
    std::unreachable();
}

void MovedWrapper::moved_event() {
    auto& player_body = global->ECS->get_component<Body>(entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);

    const auto& grav_direction = player_body.gravity_direction;
    const auto& rotation = player_body.rotation;

    //--------------------------------------------------------------------------------------
    // previous frame checking (for more responsive player controller)
    if (previous_direction == direction) {
        // allow player to hold a key and continue moving along the planet
        BodyMovement::move(entity_id, continue_in_direction);
        return;
    } else if (previous_direction == opposite_direction(direction)) {
        // also allow them to switch to the other key too without losing control
        BodyMovement::move(entity_id, opposite_direction(continue_in_direction));
        return;
    }
    //--------------------------------------------------------------------------------------

    ACCEPT filter = find_acceptance_state(rotation);

    switch (direction) {
        case DIRECTION::LEFT:
            switch (filter) {
                case ACCEPT::HORIZONTAL: {
                    BodyMovement::move(entity_id, DIRECTION::LEFT);
                    break;
                }
                case ACCEPT::HORIZONTAL_INVERT:
                    BodyMovement::move(entity_id, DIRECTION::RIGHT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::RIGHT;
                default:
                    break;
            }
            break;
        case DIRECTION::RIGHT:
            switch (filter) {
                case ACCEPT::HORIZONTAL: {
                    BodyMovement::move(entity_id, DIRECTION::RIGHT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::RIGHT;
                    break;
                }
                case ACCEPT::HORIZONTAL_INVERT:
                    BodyMovement::move(entity_id, DIRECTION::LEFT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::LEFT;
                default:
                    break;
            }
            break;
        case DIRECTION::UP:
            switch (filter) {
                case ACCEPT::VERTICAL:
                    BodyMovement::move(entity_id, DIRECTION::LEFT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::LEFT;
                    break;
                case ACCEPT::VERTICAL_INVERT:
                    BodyMovement::move(entity_id, DIRECTION::RIGHT);
                    continue_in_direction = DIRECTION::RIGHT;
                    previous_direction = direction;
                    break;
                default:
                    break;
            }
            break;
        case DIRECTION::DOWN:
            switch (filter) {
                case ACCEPT::VERTICAL:
                    BodyMovement::move(entity_id, DIRECTION::RIGHT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::RIGHT;
                    break;

                case ACCEPT::VERTICAL_INVERT:
                    BodyMovement::move(entity_id, DIRECTION::LEFT);
                    previous_direction = direction;
                    continue_in_direction = DIRECTION::LEFT;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}
