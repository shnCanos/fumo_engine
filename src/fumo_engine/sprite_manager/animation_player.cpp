#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_animation_systems.hpp"
#include "objects/components.hpp"
#include <string_view>
#include <utility>

extern std::unique_ptr<GlobalState> global;

// TODO: add system for drawing static Sprite2D
void AnimationPlayer::play(AnimationInfo& animation_info,
                           std::string_view animation_name) {

    const auto& sprite_sheet = global->sprite_manager->get_sprite_sheet(animation_name);
    if (animation_info.current_sheet_name == animation_name) [[likely]] {
        advance_animation(animation_info);
        return;
    }
    // FIXME: SIGV error is here
    PRINT(animation_name);
    PRINT(animation_info.sheet_rect_vector.size());
    PRINT(animation_info.sheet_rect_vector);
    // if we play a different animation from the current one
    // then we update our animation info
    replace_animation(animation_info, sprite_sheet);
}

void AnimationPlayer::advance_animation(AnimationInfo& animation_info) {

    animation_info.sub_counter++;
    if (animation_info.sub_counter == animation_info.frame_speed) {
        animation_info.sub_counter = 0;
        animation_info.frame_progress++;
        animation_info.current_region_rect.x += animation_info.current_region_rect.width;
    }

    // if the animation ends we remove it from the queue instead
    if (animation_info.sprite_frame_count == animation_info.frame_progress) {
        animation_info.frame_progress = 0;

        if (animation_info.sheet_rect_vector.size() > 1) [[unlikely]] {
            // play the next animation smoothly
            animation_info.sheet_rect_vector.erase(
                animation_info.sheet_rect_vector.begin());
            const auto& sprite_sheet = global->sprite_manager->get_sprite_sheet(
                animation_info.sheet_rect_vector.front().first);
            animation_info.frame_speed = sprite_sheet.base_frame_speed;
            animation_info.sprite_frame_count = sprite_sheet.sprite_frame_count;
            animation_info.current_sheet_name = sprite_sheet.sprite_sheet_name;
            animation_info.current_region_rect = sprite_sheet.base_region_rect;
        }
    }
}

void AnimationPlayer::replace_animation(AnimationInfo& animation_info,
                                        const SpriteSheet2D& sprite_sheet) {

    animation_info.sheet_rect_vector.front() = std::pair<std::string_view, Rectangle>(
        sprite_sheet.sprite_sheet_name, sprite_sheet.base_region_rect);

    animation_info.frame_progress = 0;
    animation_info.frame_speed = sprite_sheet.base_frame_speed;
    animation_info.sprite_frame_count = sprite_sheet.sprite_frame_count;
    animation_info.current_sheet_name = sprite_sheet.sprite_sheet_name;
    animation_info.current_region_rect = sprite_sheet.base_region_rect;
}

void AnimationPlayer::pause(AnimationInfo& animation_info) {
    // NOTE: not gonna implement for now unless i need it

    // const auto& sprite = global->sprite_manager->get_sprite("scarfy");
}
void AnimationPlayer::queue(AnimationInfo& animation_info,
                            std::string_view animation_name) {
    const auto& sprite_sheet = global->sprite_manager->get_sprite_sheet(animation_name);
    if (animation_info.sheet_rect_vector.size() == 1 &&
        animation_info.current_sheet_name == "NO_SHEET") [[unlikely]] {
        replace_animation(animation_info, sprite_sheet);
    }
    animation_info.sheet_rect_vector.push_back(std::pair<std::string_view, Rectangle>(
        animation_name, sprite_sheet.base_region_rect));
}
