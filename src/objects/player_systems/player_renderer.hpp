#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include "objects/renderers.hpp"

class PlayerRenderer : Renderer {
  public:
    void sys_call() override {};
    void draw_player();
};

#endif
