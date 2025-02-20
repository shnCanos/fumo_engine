#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "constants.hpp"
#include "raylib.h"

struct Body {
    Vector2 position = screenCenter;
    Vector2 velocity{0.0f, 0.0f};
    Vector2 gravity_direction = {0.0f, -1.0f};
    bool touching_ground = false;
    bool jumping = false;
    float smooth_jump_buffer{};
};

struct GravityField {

    // WARNING: counted from the surface of the object we are on
    // (need to add the radius if its a circular planet for example)
    double gravity_radius;
    float gravity_strength;

    // NOTE: i dont think mass will be used in this game
    // instead, we simply hard code the values we want on each planet for now

    // float mass;
};

struct CircleShape {
    float radius;
};

// NOTE: here for reference but we are using raylib's rectangle struct
struct RectangleShape : Rectangle {};
// Rectangle, 4 components
// typedef struct Rectangle {
//     float x;                // Rectangle top-left corner position x
//     float y;                // Rectangle top-left corner position y
//     float width;            // Rectangle width
//     float height;           // Rectangle height
// } Rectangle;

struct Render {
    Color color;
};

struct PlayerFlag {
    // used to identify the player uniquely by associating this struct
    // to an entity id
};

#endif
