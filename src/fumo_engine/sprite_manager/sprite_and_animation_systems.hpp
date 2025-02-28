#ifndef SPRITE_AND_ANIMATION_SYSTEMS_HPP
#define SPRITE_AND_ANIMATION_SYSTEMS_HPP
#include "constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include <string_view>
#include <unordered_map>

typedef Texture Texture2D;

// FIXME: finish this class
class SpriteManager {

    // holds all sprites globally, can be queried for Sprite2D or
    // SpriteSheet2Ds so they can be used by some other system
    // to implement functionality
  private:
    std::unordered_map<std::string_view, SpriteSheet2D> all_sprite_sheets{};

  public:
    void register_sprite(SpriteSheet2D sprite_sheet) {
        DEBUG_ASSERT(!all_sprite_sheets.contains(sprite_sheet.sprite_sheet_name),
                     "this sprite has already been registered.",
                     sprite_sheet.sprite_sheet_name);
        all_sprite_sheets.insert({sprite_sheet.sprite_sheet_name, sprite_sheet});
    }
    [[nodiscard]] const SpriteSheet2D& get_sprite_sheet(std::string_view sprite_name) {
        DEBUG_ASSERT(all_sprite_sheets.contains(sprite_name),
                     "this sprite hasnt been registered.", sprite_name);
        return all_sprite_sheets[sprite_name];
    }
    [[nodiscard]] const Texture2D& get_sprite_texture(std::string_view sprite_name) {
        DEBUG_ASSERT(all_sprite_sheets.contains(sprite_name),
                     "this sprite hasnt been registered.", sprite_name);
        return all_sprite_sheets[sprite_name].texture_sheet;
    }

    // FIXME: you dont need this container anymore
    // now we can just register all our sprite sheets without making them into
    // components -> because AnimationInfos are unique per entity id and hold
    // all the info we need to operate on SpriteSheet2D

    // NamedComponentContainer<SpriteSheet2D> all_sprite_sheets{};
    void unload_all_textures() {
        for (auto& sprite_sheet_pair : all_sprite_sheets) {
            auto& sprite_sheet = sprite_sheet_pair.second;
            UnloadTexture(sprite_sheet.texture_sheet);
        }
    }
};

// unregistered unscheduled
// FIXME: finish this class
class AnimationPlayer : public System {
    // has all of the functionality associated with AnimationInfo
    // and alters the AnimationInfo component
  public:
    void sys_call() override {}

    // NOTE: ALWAYS replaces the current animation with the new one
    void play(AnimationInfo& animation_info, std::string_view animation_name);
    void queue(AnimationInfo& animation_info,
                            std::string_view animation_name);
    // FIXME: do these later
    // ---------------------------------------------------------------------------

    // pauses the current animation and resets it to the initial state
    void pause(AnimationInfo& animation_info);
    void resume(AnimationInfo& animation_info);
    void stop(AnimationInfo& animation_info);
    // void play_after(std::string_view play_after_animation,
    //                 std::string_view target_animation);

    // Clears all queued, unplayed animations.
    void clear_queue();
    // FIXME:
    // ---------------------------------------------------------------------------

  private:
    void advance_animation(AnimationInfo& animation_info);
    void replace_animation(AnimationInfo& animation_info,
                           const SpriteSheet2D& sprite_sheet);
};

struct AnimationRenderer : System {

    void sys_call() override { draw_animations(); }
    void draw_animations();

  private:
    void draw_animation(const AnimationInfo& animation_info,
                        const Texture2D& sheet_texture, const Body& body);
};
#endif
