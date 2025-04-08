#pragma once
#include <string_view>
#include <unordered_map>

#include "constants.hpp"
#include "fumo_engine/components.hpp"
#include "fumo_engine/core/system_base.hpp"

typedef Texture Texture2D;

// TODO: finish this class
class SpriteManager {
    // holds all sprites globally, can be queried for Sprite2D or
    // SpriteSheet2Ds so they can be used by some other system
    // to implement functionality
  private:
    std::unordered_map<std::string_view, SpriteSheet2D> all_sprite_sheets {};

  public:
    void register_sprite(SpriteSheet2D sprite_sheet) {
        DEBUG_ASSERT(!all_sprite_sheets.contains(sprite_sheet.sprite_sheet_name),
                     "this sprite has already been registered.",
                     sprite_sheet.sprite_sheet_name);
        all_sprite_sheets.insert({sprite_sheet.sprite_sheet_name, sprite_sheet});
    }

    [[nodiscard]] const SpriteSheet2D&
    get_sprite_sheet(std::string_view sprite_name) {
        DEBUG_ASSERT(all_sprite_sheets.contains(sprite_name),
                     "this sprite hasnt been registered.",
                     sprite_name);
        return all_sprite_sheets[sprite_name];
    }

    [[nodiscard]] const Texture2D& get_sprite_texture(std::string_view sprite_name) {
        DEBUG_ASSERT(all_sprite_sheets.contains(sprite_name),
                     "this sprite hasnt been registered.",
                     sprite_name);
        return all_sprite_sheets[sprite_name].texture_sheet;
    }

    void unload_all_textures() {
        for (auto& sprite_sheet_pair : all_sprite_sheets) {
            auto& sprite_sheet = sprite_sheet_pair.second;
            UnloadTexture(sprite_sheet.texture_sheet);
        }
    }
};

namespace AnimationPlayer {
// NOTE: ALWAYS replaces the current animation with the new one
void play(AnimationInfo& animation_info, std::string_view animation_name);
// void play(AnimationInfo& animation_info, std::string_view animation_name,
//           bool looping);
void queue(AnimationInfo& animation_info, std::string_view animation_name);
// ---------------------------------------------------------------------------

// pauses the current animation and resets it to the initial state
void pause(AnimationInfo& animation_info);
void resume(AnimationInfo& animation_info);
void stop(AnimationInfo& animation_info);
// void play_after(std::string_view play_after_animation,
//                 std::string_view target_animation);

// Clears all queued, unplayed animations.
void clear_queue();

void advance_animation(AnimationInfo& animation_info,
                       const SpriteSheet2D& sprite_sheet);
void replace_animation(AnimationInfo& animation_info,
                       const SpriteSheet2D& sprite_sheet);
}; // namespace AnimationPlayer

struct AnimationRenderer: System {
    void sys_call() override {
        draw_animations();
    }

    void draw_animations();

  private:
    void draw_animation(const AnimationInfo& animation_info,
                        const Texture2D& sheet_texture,
                        const Body& body);
};

struct EntireAnimationPlayer: System {
    AnimationInfo* animation_info_ptr;
    std::string_view animation_name = "NO_NAME";

    void sys_call() override {
        play_full_animation();
    }

    // NOTE: this system is sleeping on already slept systems
    // if i add the check it crashes the game
    void play_entire_animation(AnimationInfo& _animation_info,
                               std::string_view _animation_name) {
        animation_info_ptr = &_animation_info;
        animation_name = _animation_name;
    };

  private:
    void play_full_animation();
};

