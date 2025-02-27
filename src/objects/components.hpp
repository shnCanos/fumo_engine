#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "constants.hpp"
#include "raylib.h"
#include "raymath.h"
#include <string_view>
#include <utility>
struct Rectangle;

struct Body {
    int iterations{};
    int count{};
    Vector2 position = screenCenter;
    Vector2 velocity{0.0f, 0.0f};
    Vector2 gravity_direction = {0.0f, -1.0f};
    bool touching_ground = false;
    bool jumping = false;
    bool going_up;
    bool going_down;
    float smooth_jump_buffer{};
    // NOTE: follows the gravity direction, not the vertical player direction
    [[nodiscard]] Vector2 get_y_velocity() {
        return gravity_direction * Vector2DotProduct(velocity, gravity_direction);
    }
    [[nodiscard]] float get_dot_y_velocity() {
        return Vector2DotProduct(velocity, gravity_direction);
    }
    [[nodiscard]] float get_dot_x_velocity() {
        return Vector2DotProduct(velocity, {gravity_direction.y, -gravity_direction.x});
    }
    // void set_y_velocity(float new_y_velocity) {
    //     gravity_direction* Vector2DotProduct(velocity, gravity_direction);
    // }
    void scale_velocity(float scale) { velocity += gravity_direction * scale; }
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
// struct Rectangle : Rectangle {
//     Rectangle(float width, float height) : Rectangle(width, height) {}
// };
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

struct AnimationInfo {

    // keeps track of an entity's animation
    // so we can update that entity's SpriteSheet2D
    // a vector to hold the animations we want to play for the entity
    std::vector<std::pair<std::string_view, Rectangle>> sheet_rect_vector{};
    // NOTE: it is a bit of overhead, but i am following godot's implementation
    // for this. for now i do not need this kind of behavior, but i will for stuff
    // like a landing animation being queued after a jump.
    // we also want to be able to replace the head element
    int frame_progress{}; // progress along the frame
    int frame_speed{};    // number of frames to show per second
    int sprite_frame_count{};
    std::string_view current_sheet_name = "NO_SHEET";
    Rectangle current_region_rect{};
    int sub_counter{};

    AnimationInfo() {
        sheet_rect_vector.push_back(
            std::pair<std::string_view, Rectangle>("NO_SHEET", Rectangle{0, 0, 0, 0}));
    }
};
// ------------------------------------------------------------------------
// NOTE: not used as components
// ------------------------------------------------------------------------
struct Sprite2D {
    Texture2D texture;
    std::string_view sprite_name;
    Rectangle region_rect;
};

// NOTE: when making an animation, we created an EntityId for that animation,
// which we then attribute to whatever entity should hold that animation

struct SpriteSheet2D {
    // used to animate entities
    Texture2D texture_sheet;
    std::string_view sprite_sheet_name;
    int sprite_frame_count;
    int base_frame_speed{}; // number of frames to show per second
    Rectangle base_region_rect{.width = (float)texture_sheet.width / sprite_frame_count,
                               .height = (float)texture_sheet.height};
    // base rect that defines how we go through each animation frame
};

#endif
