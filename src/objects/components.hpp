#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "raylib.h"

struct Body {
    Vector2 position;
    Vector2 velocity;
    float mass;
};

struct CircleShape {
    float radius;
};

struct Render {
    Color color;
};
// struct Mouse {
//     Vector2 position;
//     float mouse_radius{50};
//     // Render render;
// };
#endif
