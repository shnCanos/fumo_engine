#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP
#include "constants.hpp"
#include "fumo_engine/core/engine_constants.hpp"
#include "raylib.h"
#include "raymath.h"
#include <string_view>
struct Rectangle;

struct Body {
    int iterations{};
    int count{};
    Vector2 position = screenCenter;
    Vector2 velocity{0.0f, 0.0f};
    Vector2 gravity_direction = {0.0f, -1.0f};
    Vector2 x_direction = {gravity_direction.y, -gravity_direction.x};
    bool inverse_direction = false;
    bool touching_ground = true;
    bool jumping = false;
    bool going_up;
    bool going_down;
    float smooth_jump_buffer{};

    float rotation{}; // NOTE: in degrees?

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
};
struct AggregateField {
    // used to flag planets for orbit swap checking
    // use if we want to transition based on proximity
};
struct Circle {
    float radius;
};

struct PlayerShape {
    EntityId player_owning_field = NO_ENTITY_FOUND;

    float radius;
    Vector2 top_circle_center;
    Vector2 bottom_circle_center;

    std::pair<Vector2, Vector2> left_line;  // .first is the bottom point
    std::pair<Vector2, Vector2> right_line; // .first is the bottom point

    void update_capsule_positions(const Body& player_body) {
        top_circle_center =
            player_body.position - player_body.gravity_direction * radius;
        bottom_circle_center =
            player_body.position + player_body.gravity_direction * radius;

        left_line.second = top_circle_center - player_body.x_direction * radius;
        left_line.first = bottom_circle_center - player_body.x_direction * radius;

        right_line.second = top_circle_center + player_body.x_direction * radius;
        right_line.first = bottom_circle_center + player_body.x_direction * radius;
    }
};

// WARNING: **NOT** counted from the surface of the object we are on
struct ParallelGravityField {
    // this is parallel to a surface and points in one direction
    double gravity_reach{};
    float gravity_strength{};
    // Vector2 position = screenCenter;
    float rotation{}; // in degrees

    Rectangle field_rectangle{};

    bool is_inside_field(const Body& player_body, const PlayerShape& player_shape) const;
};

// WARNING: **NOT** counted from the surface of the object we are on
struct CircularGravityField {
    double gravity_radius;
    float gravity_strength;
    Vector2 position = screenCenter;

    bool is_inside_field(const Body& player_body, const PlayerShape& player_shape) const;
};

struct Level1Tag {
    // identify objects in level 1
    // NOTE: general idea for how levels work:
    //
    // we create all the entities a level will need, then we store them in some
    // level manager thing that deals with them, and knows what belongs where
    // it also adds the OnScreen component to entities so other systems
    // can know what entities they should be updating
    // and it manages the screen transitions
};

struct OnScreen {
    // given to entities that are currently on screen
    // (used by systems that only care about what should be on the screen)
};

struct Render {
    Color color;
};

struct PlayerFlag {
    // used to identify the player uniquely by associating this struct
    // to an entity id
};

// NOTE: to make a really modular and reusable timer class,
// consider using std::function to send in a function call
// after the timer ends

struct Timer {
    friend struct TimerHandler;

    float seconds_duration;
    std::string_view system_name = "NO_NAME";

    void make_timer(float _duration_seconds, std::string_view _system_name) {
        seconds_duration = _duration_seconds;
        system_name = _system_name;

        starting_time = GetTime();
        ending_time = starting_time + seconds_duration;
    }

    // another templated constructor we might wanna use in the future
    // template<typename T>
    // Timer(float duration_seconds) : duration_seconds(duration_seconds) {
    //     system_name = libassert::type_name<T>();
    //
    //     starting_time = GetTime();
    //     ending_time = starting_time + duration_seconds;
    // }

  private:
    float starting_time;
    float ending_time;
};

struct AnimationInfo {

    // keeps track of an entity's animation
    // so we can update that entity's SpriteSheet2D
    // a vector to hold the animations we want to play for the entity
    std::vector<std::string_view> sheet_vector{std::string_view("NO_SHEET")};
    // NOTE: it is a bit of overhead, but i am following godot's implementation
    // for this. for now i do not need this kind of behavior, but i will for stuff
    // like a landing animation being queued after a jump.
    // we also want to be able to replace the head element
    int frame_progress{1}; // progress along the frame
    int frame_speed{};     // number of frames to show per second
    int sprite_frame_count{};
    std::string_view current_sheet_name = "NO_SHEET";
    Rectangle current_region_rect{};
    int sub_counter{};
    bool is_running = false;
};
// ------------------------------------------------------------------------
// NOTE: not used as components
// ------------------------------------------------------------------------

// NOTE: when making an animation, we created an EntityId for that animation,
// which we then attribute to whatever entity should hold that animation

struct SpriteSheet2D {
    // used to animate entities
    Texture2D texture_sheet;
    std::string_view sprite_sheet_name;
    int sprite_frame_count;
    int base_frame_speed{}; // number of frames to show per second
    Rectangle base_region_rect{.x = 0.0f,
                               .y = 0.0f,
                               .width = (float)texture_sheet.width / sprite_frame_count,
                               .height = (float)texture_sheet.height};
    // base rect that defines how we go through each animation frame
    bool looping = false;
};

struct Sprite2D {
    Texture2D texture;
    std::string_view sprite_name;
    Rectangle region_rect;
};
#endif
