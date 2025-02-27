#ifndef SPRITE_AND_TEXTURE_SYSTEMS_HPP
#define SPRITE_AND_TEXTURE_SYSTEMS_HPP
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include "objects/fumo_containers.hpp"
#include <string_view>

struct GlobalSpriteHandler : System {
    // unregistered scheduled
    // textures are stored in rectangles in the case of sprites
    // (this class is a massive massive part of the engine,
    // and will need frequent new methods as we progress the game)
    // mainly will need updates as we try adding fancy effects that need to be
    // rendered

    void sys_call() override {}
    // FIXME: implement all of the texture loading functions for initialization of the
    // game
    void global_texture_load();
    void unload_all_textures();
    // specific segmentation of loads
    void load_player_textures();
};

class SpriteManager {

    // holds all sprites globally, can be queried for Sprite2D or
    // SpriteSheet2Ds so they can be used by some other system
    // to implement functionality

  public:
    void initialize();

  private:
    NamedComponentContainer<SpriteSheet2D> all_sprite_sheets{};
};

// unregistered unscheduled
// FIXME: finish this class
class AnimationPlayer : public System {
    // has all of the functionality associated with AnimationInfo
    // and alters the AnimationInfo component
  public:
    void sys_call() override {}

    // NOTE: ALWAYS replaces the current animation with the new one
    void play(AnimationInfo animation_info, std::string_view animation_name);
    void resume(AnimationInfo animation_info);
    void pause(AnimationInfo animation_info);

    // pauses the current animation and resets it to the initial state
    void stop(AnimationInfo animation_info);

    // FIXME: implement queue_animation() later for fancier transitions
    void queue(std::string_view animation_name, AnimationInfo animation_info);
    void play_after(std::string_view play_after_animation,
                    std::string_view target_animation);

    // Clears all queued, unplayed animations.
    void clear_queue();
};

#endif
