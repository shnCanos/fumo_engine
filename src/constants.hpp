#pragma once
#include <cereal/archives/json.hpp>
#include <iostream> // IWYU pragma: export
#include <libassert/assert.hpp>

#include "array"
#include "fumo_raylib.hpp"
// const int screenWidth = 500;
constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr FumoVec2 screenCenter = {(float)screenWidth / 2, (float)screenHeight / 2};
#define GRAVITY 9.81
#define GRAVITATIONAL_CONSTANT 0.6
#define SCALING_FACTOR 1
#define FONT_SIZE 20
const float mouse_radius = 120.0f;
const int color_count = 15;
const float default_mass = 600000;
const float default_radius = 50.0f;
const float default_planet_radius = 150.0f;
constexpr FumoVec2 starter_player_position = {.x = 1920.0f / 2.0f,
                                              .y = screenCenter.y};
const float max_fall_velocity = 500.0f;
// const float default_grav_strength = 1000.0f;
constexpr FumoColor player_color = FUMO_ORANGE;
// NOTE: add orange back to all colors after adding a sprite to the player
const std::array<FumoColor, 100> all_colors = {FUMO_LIGHTGRAY,
                                               FUMO_GRAY,
                                               FUMO_YELLOW,
                                               FUMO_GOLD,
                                               FUMO_PINK,
                                               FUMO_RED,
                                               FUMO_MAROON,
                                               FUMO_GREEN,
                                               FUMO_LIME,
                                               FUMO_SKYBLUE,
                                               FUMO_BLUE,
                                               FUMO_PURPLE,
                                               FUMO_VIOLET,
                                               FUMO_BEIGE,
                                               FUMO_ORANGE};
#define PRINT(x) \
    std::cerr << #x << " ---> " << libassert::highlight_stringify(x) << '\n';

//---------------------------------------------------------------------------
// fumo engine primitive types
