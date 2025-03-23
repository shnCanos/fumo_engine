
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"
#include "objects/generic_systems/systems.hpp"

extern std::unique_ptr<GlobalState> global;

namespace EventHandler {

void jumped(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.on_ground) {
        BodyMovement::jump(player_body);
    }
}

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

void move_up(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    BodyMovement::move_vertically(player_body, 1.0f);
}

void move_down(const Event& event) {
    auto& player_body = global->ECS->get_component<Body>(event.entity_id);
    BodyMovement::move_vertically(player_body, -1.0f);
}

void idle(const Event& event) {
    auto& player_animation = global->ECS->get_component<AnimationInfo>(event.entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(event.entity_id);

    if (player_state.on_ground) {
        AnimationPlayer::play(player_animation, "idle");
    }
}

} // namespace EventHandler
