#ifndef RENDERERS_HPP
#define RENDERERS_HPP
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
typedef Texture Texture2D;

struct PlanetRenderer : System {
  public:
    void sys_call() override { draw_planet(); };
    void draw_planet();
};

struct AnimationRenderer : System {

    void sys_call() override { draw_animations(); }
    void draw_animations();

  private:
    void draw_animation(const AnimationInfo& animation_info,
                        const Texture2D& sheet_texture, const Body& body);
};

#endif
