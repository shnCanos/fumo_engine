#include <string_view>

#include "constants.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/core/scheduling_systems.hpp"
#include "fumo_engine/sprite_animation_manager/sprite_and_animation_systems.hpp"

extern std::unique_ptr<GlobalState> global;
void debug_print_animation_info(const AnimationInfo& animation_info);

// TODO: add system for drawing static Sprite2D
void AnimationPlayer::play(
    AnimationInfo& animation_info,
    std::string_view animation_name
) {
    const auto& sprite_sheet =
        global->sprite_manager->get_sprite_sheet(animation_name);

    if (animation_info.current_sheet_name == animation_name
        && animation_info.is_running) [[likely]] {
        advance_animation(animation_info, sprite_sheet);
        return;
    }
    // if we play a different animation from the current one
    // then we update our animation info
    replace_animation(animation_info, sprite_sheet);
}

void AnimationPlayer::advance_animation(
    AnimationInfo& animation_info,
    const SpriteSheet2D& sprite_sheet
) {
    animation_info.sub_counter++;
    if (animation_info.sub_counter >= animation_info.frame_speed) {
        animation_info.sub_counter = 0;
        animation_info.current_region_rect.x =
            animation_info.current_region_rect.width
            * animation_info.frame_progress;
        animation_info.frame_progress++;
    }

    // if the animation ends we remove it from the queue instead
    if (animation_info.frame_progress == animation_info.sprite_frame_count) {
        if (animation_info.sheet_vector.size() > 1) [[unlikely]] {
            // play the next animation smoothly
            animation_info.sheet_vector.erase(animation_info.sheet_vector.begin(
            ));

            const auto& sprite_sheet = global->sprite_manager->get_sprite_sheet(
                animation_info.sheet_vector.front()
            );

            animation_info.frame_speed = sprite_sheet.base_frame_speed;
            animation_info.sprite_frame_count = sprite_sheet.sprite_frame_count;
            animation_info.current_sheet_name = sprite_sheet.sprite_sheet_name;
            animation_info.current_region_rect = sprite_sheet.base_region_rect;
        }
        if (sprite_sheet.looping) {
            animation_info.frame_progress = 1;
            animation_info.current_region_rect.x = 0;
            return;
        }
        animation_info.is_running = false;
    }
}

void AnimationPlayer::replace_animation(
    AnimationInfo& animation_info,
    const SpriteSheet2D& sprite_sheet
) {
    animation_info.sheet_vector.front() = sprite_sheet.sprite_sheet_name;

    animation_info.frame_progress = 1;
    animation_info.frame_speed = sprite_sheet.base_frame_speed;
    animation_info.sprite_frame_count = sprite_sheet.sprite_frame_count;
    animation_info.current_sheet_name = sprite_sheet.sprite_sheet_name;
    animation_info.current_region_rect = sprite_sheet.base_region_rect;
    animation_info.is_running = true;
}

void AnimationPlayer::pause(AnimationInfo& animation_info) {
    // NOTE: not gonna implement for now unless i need it
}

void AnimationPlayer::queue(
    AnimationInfo& animation_info,
    std::string_view animation_name
) {
    const auto& sprite_sheet =
        global->sprite_manager->get_sprite_sheet(animation_name);
    if (animation_info.sheet_vector.size() == 1
        && animation_info.current_sheet_name == "NO_SHEET") [[unlikely]] {
        replace_animation(animation_info, sprite_sheet);
    }
    animation_info.sheet_vector.push_back(animation_name);
}

//  ---------------------------------------------------------------------------
void EntireAnimationPlayer::play_full_animation() {
    DEBUG_ASSERT(
        animation_name != "NO_NAME",
        "forgot to add an animation before awaking this system."
    );

    AnimationPlayer::play(*animation_info_ptr, animation_name);

    if (!animation_info_ptr->is_running) {
        animation_name = "NO_NAME";

        const auto& scheduler_system =
            global->ECS->get_system<SchedulerSystemECS>();
        scheduler_system->sleep_unregistered_system<EntireAnimationPlayer>();
    }
}
