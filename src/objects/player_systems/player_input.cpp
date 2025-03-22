#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/sprite_animation_manager/sprite_and_animation_systems.hpp"
#include "fumo_engine/components.hpp"
#include "objects/player_systems/player_general_systems.hpp"
#include "objects/generic_systems/systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

void PlayerInputHandler::handle_input() {
    const auto body_movement_ptr = global->ECS->get_system<BodyMovement>();
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(player_id);

    const auto& animation_player_ptr = global->ECS->get_system<AnimationPlayer>();

    bool idle = true;
    //  FIXME:
    //  ---------------------------------------------------------------------------
    //  turn all these if checks into an event class separate from this class
    //  ---------------------------------------------------------------------------

    if (player_body.jumping) {
        if (player_animation.frame_progress != player_animation.sprite_frame_count) {
            animation_player_ptr->play(player_animation, "jump");
        }
        idle = false;
    }

    if (IsKeyDown(KEY_SPACE) && player_body.on_ground) {

        body_movement_ptr->jump(player_body);
        animation_player_ptr->play(player_animation, "jump");
        idle = false;
    }
    if (IsKeyDown(KEY_DOWN)) {
        body_movement_ptr->move_vertically(player_body, -1.0f);
        idle = false;
    }
    if (IsKeyDown(KEY_UP)) {
        body_movement_ptr->move_vertically(player_body, 1.0f);
        idle = false;
    }
    if (IsKeyDown(KEY_LEFT)) {
        if (player_body.on_ground) {
            animation_player_ptr->play(player_animation, "run_backwards");
        }
        idle = false;
        body_movement_ptr->move_horizontally(player_body, -1.0f);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (player_body.on_ground) {
            animation_player_ptr->play(player_animation, "run");
            idle = false;
        }
        body_movement_ptr->move_horizontally(player_body, 1.0f);
    }

    if (idle && player_body.on_ground) {
        animation_player_ptr->play(player_animation, "idle");
    }
    if (!player_body.jumping && !player_body.on_ground) {
        // TODO: debug
        // hardcoded, remove this later
        // make it look like player is falling

        animation_player_ptr->play(player_animation, "jump");

        player_animation.frame_progress = 3;
        player_animation.current_region_rect.x =
            player_animation.current_region_rect.width * player_animation.frame_progress;
        player_animation.is_running = true;
    }
}
// const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
// scheduler_system
//     ->awake_unregistered_system_priority<EntireAnimationPlayer, 58>();
//
// const auto& entire_anim_player =
//     global->ECS->get_system<EntireAnimationPlayer>();
// entire_anim_player->play_entire_animation(player_animation, "jump");
