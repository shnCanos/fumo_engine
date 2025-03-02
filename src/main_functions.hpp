
#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP
#include "objects/components.hpp"
#include "raylib.h"
Vector2 Vector2ScaleDivision(Vector2 v, float scale);
void debug_print_animation_info(const AnimationInfo& animation_info);
float PointToLineDistance(Vector2& Point, Vector2& LineStart, Vector2& LineEnd);

#endif
