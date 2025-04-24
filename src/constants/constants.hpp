#pragma once
#include <cereal/archives/json.hpp>
#include <iostream> // IWYU pragma: export
#include <libassert/assert.hpp>

#include "array"
#include "fumo_raylib.hpp"
#define GRAVITY 9.81
#define GRAVITATIONAL_CONSTANT 0.6
#define SCALING_FACTOR 1
#define FONT_SIZE 20

const int color_count = 15;
// const float default_mass = 600000;
// const float max_fall_velocity = 500.0f;
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

//---------------------------------------------------------------------------
// fumo engine primitive types
