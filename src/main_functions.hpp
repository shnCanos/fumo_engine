
#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP
#include "fumo_engine/components.hpp"
#include "objects/generic_systems/systems.hpp"
#include "raylib.h"
Vector2 Vector2ScaleDivision(Vector2 v, float scale);
void debug_print_animation_info(const AnimationInfo& animation_info);
void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player);
void debug_player_drawing(const PlayerShape& player_shape, const Body& player_body);
DIRECTION opposite_direction(DIRECTION direction);
#endif
