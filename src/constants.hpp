#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_
#include "array"
#include "raylib.h"
// WARNING:
// ----------------------------------------------------------------
#include <iostream>
// WARNING: ignore clangd's warning, you need to include <iostream>
// ----------------------------------------------------------------
#include <libassert/assert.hpp>
// const int screenWidth = 500;
constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr Vector2 screenCenter = {(float)screenWidth / 2, (float)screenHeight / 2};
#define GRAVITY 9.81
#define GRAVITATIONAL_CONSTANT 0.6
#define SCALING_FACTOR 1
#define FONT_SIZE 20
const float mouse_radius = 120.0f;
const int color_count = 15;
const float default_mass = 600000;
const float default_radius = 50.0f;
const float default_planet_radius = 150.0f;
constexpr Vector2 starter_player_position = {.x = 1920.0f / 2.0f,
                                             .y = screenCenter.y};
const float max_fall_velocity = 500.0f;
// const float default_grav_strength = 1000.0f;
constexpr Color player_color = ORANGE;
// NOTE: add orange back to all colors after adding a sprite to the player
const std::array<Color, 100> all_colors = {LIGHTGRAY,
                                           GRAY,
                                           YELLOW,
                                           GOLD,
                                           PINK,
                                           RED,
                                           MAROON,
                                           GREEN,
                                           LIME,
                                           SKYBLUE,
                                           BLUE,
                                           PURPLE,
                                           VIOLET,
                                           BEIGE,
                                           ORANGE};
#define PRINT(x) \
    std::cerr << #x << " ---> " << libassert::highlight_stringify(x) << '\n';
#endif
