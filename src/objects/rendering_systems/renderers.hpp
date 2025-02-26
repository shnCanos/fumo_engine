#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include "objects/fumo_containers.hpp"
#include <string_view>

struct PlanetRenderer : System {
  public:
    void sys_call() override { draw_planet(); };
    void draw_planet();
};

struct GlobalRenderer : System {
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
};

class PlayerRenderer : public System {

  Sprite2DAnimations sprite_animations;
  Sprite2D player_sprite;

  public:
    void sys_call() override {}
    // FIXME: implement play_animation()
    void play_animation(std::string_view animation_name);

    void add_sprite_sheet(EntityId sprite_sheet_id, std::string_view animation_name) {
        // player_animations.add_entity_id<SpriteSheet2D>(sprite_sheet_id,
        // animation_name);
    }
    void destroy_sprite_sheet_by_name(std::string_view animation_name) {
        // player_animations.destroy_entity_id_by_name(animation_name);
    }
    void remove_sprite_sheet_by_name(std::string_view animation_name) {
        // player_animations.remove_entity_id_by_name(animation_name);
    }

  private:
    // FIXME: implement loading player textures
    void load_player_textures();
};

class AnimationPlayer : public System {
  public:
    void sys_call() override {}
    // FIXME: implement play_animation()
    void play_animation(SpriteSheet2D animation_sheet);
};

#endif
