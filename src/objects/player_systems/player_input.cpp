#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/sprite_animation_manager/sprite_and_animation_systems.hpp"
#include "objects/generic_systems/systems.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

void PlayerInputHandler::handle_input() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    auto& player_animation =
        global->ECS->get_component<AnimationInfo>(player_id);
    auto& player_state =
        global->ECS->get_component<EntityState>(global->player_id);

    int key_count = 0;

    if (IsKeyDown(KEY_SPACE) && player_state.on_ground) {
        BodyMovement::jump(player_body);
        key_count++;
    }
    if (IsKeyDown(KEY_DOWN)) {
        BodyMovement::move_vertically(player_body, -1.0f);
        key_count++;
    }
    if (IsKeyDown(KEY_UP)) {
        BodyMovement::move_vertically(player_body, 1.0f);
        key_count++;
    }
    if (IsKeyDown(KEY_LEFT)) {
        global->event_handler->add_event(
            {.event = EVENT_::ENTITY_MOVED_LEFT, .entity_id = player_id}
        );

        key_count++;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        global->event_handler->add_event(
            {.event = EVENT_::ENTITY_MOVED_RIGHT, .entity_id = player_id}
        );
        key_count++;
    }

    if (key_count == 0 && player_state.on_ground) {
        AnimationPlayer::play(player_animation, "idle");
    }

    if (!player_state.jumping && !player_state.on_ground) {
        AnimationPlayer::play(player_animation, "jump");

        player_animation.frame_progress = 3;
        player_animation.current_region_rect.x =
            player_animation.current_region_rect.width
            * player_animation.frame_progress;
        player_animation.is_running = true;
    }
}
