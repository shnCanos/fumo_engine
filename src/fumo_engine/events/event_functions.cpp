
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"
#include "objects/generic_systems/systems.hpp"

extern std::unique_ptr<GlobalState> global;

namespace EventHandler {

void move_left(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.on_ground) {
        AnimationPlayer::play(player_animation, "run_backwards");
    }
    BodyMovement::move_horizontally(player_body, -1.0f);
}

void move_right(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.on_ground) {
        AnimationPlayer::play(player_animation, "run");
    }
    BodyMovement::move_horizontally(player_body, 1.0f);
}

void jumped(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.jumping) {
        if (player_animation.frame_progress != player_animation.sprite_frame_count) {
            AnimationPlayer::play(player_animation, "jump");
        }
    }
}

} // namespace EventHandler
