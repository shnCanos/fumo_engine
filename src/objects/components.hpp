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
