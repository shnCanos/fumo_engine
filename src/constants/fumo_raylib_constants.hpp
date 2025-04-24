
#pragma once
#include "fumo_raylib.hpp"
constexpr static int screenWidth = 1920;
constexpr static int screenHeight = 1080;
constexpr static FumoVec2 screenCenter = {(float)screenWidth / 2.0f,
                                          (float)screenHeight / 2.0f};

constexpr FumoVec2 starter_player_position = {.x = 1920.0f / 2.0f,
                                              .y = screenCenter.y};

const float mouse_radius = 120.0f;
