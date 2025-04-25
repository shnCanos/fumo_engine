#include <sched.h>

#include <cerrno>

#include "constants/movement_constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/generic_systems/systems.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

void BodyMovement::move(const EntityId& entity_id, const DIRECTION& direction) {
    auto& body = fumo_engine->ECS->get_component<Body>(entity_id);
    auto& player_state =
        fumo_engine->ECS->get_component<EntityState>(entity_id);
    auto& player_animation =
        fumo_engine->ECS->get_component<AnimationInfo>(entity_id);
    switch (direction) {
        case DIRECTION::LEFT:
            body.velocity -= body.real_x_direction * MOVEMENT_SCALING;
            if (player_state.on_ground) {
                AnimationPlayer::play(player_animation, "run");
            }
            break;
        case DIRECTION::RIGHT:
            body.velocity += body.real_x_direction * MOVEMENT_SCALING;
            if (player_state.on_ground) {
                AnimationPlayer::play(player_animation, "run");
            }
            break;

        default:
            PANIC("UNREACHABLE");
            std::unreachable();
    }
}

void BodyMovement::jump(Body& body, const EntityId& entity_id) {
    auto& player_state =
        fumo_engine->ECS->get_component<EntityState>(entity_id);
    // TODO:
    // Bonus Air Time
    // Peak control
    // Fast Falling
    // Lerp the jump movement

    body.velocity = body.get_real_x_velocity()
        + FumoVec2Negate(body.real_gravity_direction) * JUMP_SCALING;
}
