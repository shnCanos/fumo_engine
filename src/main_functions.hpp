#pragma once
#include "fumo_engine/components.hpp"
#include "objects/generic_systems/systems.hpp"
#include "raylib.h"
void debug_print_animation_info(const AnimationInfo& animation_info);
void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player);
void debug_player_drawing(const Capsule& player_shape,
                          const Body& player_body);
DIRECTION opposite_direction(DIRECTION direction);
[[nodiscard]] FumoVec2 direction_to_vector(DIRECTION direction);
