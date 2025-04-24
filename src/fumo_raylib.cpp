#include "fumo_raylib.hpp"

void FumoDrawCircleV(FumoVec2 center, float radius, FumoColor color) {
    DrawCircleV(center.to_raylib_vec2(), radius, color.to_raylib_color());
}

void FumoDrawLineV(FumoVec2 StartPos, FumoVec2 EndPos, FumoColor color) {
    DrawLineV(StartPos.to_raylib_vec2(),
              EndPos.to_raylib_vec2(),
              color.to_raylib_color());
}

void FumoDrawLineEx(FumoVec2 StartPos,
                    FumoVec2 EndPos,
                    float thick,
                    FumoColor color) {
    DrawLineEx(StartPos.to_raylib_vec2(),
               EndPos.to_raylib_vec2(),
               thick,
               color.to_raylib_color());
}

void FumoDrawRectV(FumoVec2 position, FumoVec2 size, FumoColor color) {
    DrawRectangleV(position.to_raylib_vec2(),
                   size.to_raylib_vec2(),
                   color.to_raylib_color());
}
