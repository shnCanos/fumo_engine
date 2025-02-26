#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"

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
    void global_texture_load();
    void unload_all_textures();

  private:
    void load_player_textures();
};

class PlayerRenderer : public System {

  SpriteSheet2D running_animation;
  Sprite2D player_sprite;

  public:
    void sys_call() override {}
    void draw_player_static();
    void draw_running_animation();
    void draw_jumping_animation();
};


class AnimationPlayer : public System {

public:
    void sys_call() override {}


};

#endif
