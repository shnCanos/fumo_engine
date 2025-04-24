#pragma once
#include "constants/movement_constants.hpp"
#include "fumo_engine/components.hpp"
#include "raylib.h"

void debug_print_animation_info(const AnimationInfo& animation_info);
void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player);
void debug_player_drawing(const Capsule& player_shape, const Body& player_body);
