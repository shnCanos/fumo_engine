#pragma once
#include "fumo_serialize_macros.hpp"
#include "raylib.h"
#include "raymath.h"

struct FumoVec2 {
    float x; // Vector x component
    float y; // Vector y component

    inline Vector2 to_raylib_vec2() const {
        return {x, y};
    }
    SERIALIZE(x, y)
};

struct FumoColor {
    unsigned char r; // FumoColor red value
    unsigned char g; // FumoColor green value
    unsigned char b; // FumoColor blue value
    unsigned char a; // FumoColor alpha value

    inline Color to_raylib_color() const {
        return {r, g, b, a};
    }
    SERIALIZE(r, g, b, a)
};

inline FumoColor to_fumo_color(Color c) {
    return {c.r, c.g, c.b, c.a};
}

struct FumoRect {
    // here because raylib is c and i cant add a damn template to the source code
    SERIALIZE(x, y, width, height)
    float x; // FumoRect top-left corner position x
    float y; // FumoRect top-left corner position y
    float width; // FumoRect width
    float height; // FumoRect height

    Rectangle to_raylib_rect() {
        return {x, y, width, height};
    }
};

void FumoDrawCircleV(FumoVec2 center, float radius, FumoColor color);
void FumoDrawLineV(FumoVec2 TopLeft, FumoVec2 TopRight, FumoColor color);
void FumoDrawRectV(FumoVec2 position, FumoVec2 size, FumoColor color);

inline FumoColor FumoColorAlpha(FumoColor color, float alpha) {
    return to_fumo_color(ColorAlpha(color.to_raylib_color(), alpha));
}

inline FumoVec2 FumoVec2ScaleDivision(FumoVec2 v, float scale) {
    FumoVec2 result = {v.x / scale, v.y / scale};
    return result;
}

inline float FumoVec2DotProduct(FumoVec2 v1, FumoVec2 v2) {
    return Vector2DotProduct(v1.to_raylib_vec2(), v1.to_raylib_vec2());
}

inline float FumoVec2Distance(FumoVec2 v1, FumoVec2 v2) {
    return Vector2Distance(v1.to_raylib_vec2(), v2.to_raylib_vec2());
}

inline float FumoVec2DistanceSqr(FumoVec2 v1, FumoVec2 v2) {
    return Vector2DistanceSqr(v1.to_raylib_vec2(), v2.to_raylib_vec2());
}

inline FumoVec2 to_fumo_vec2(Vector2 v) {
    return {v.x, v.y};
}

inline FumoVec2 FumoVec2Add(FumoVec2 lhs, FumoVec2 rhs) {
    return to_fumo_vec2(Vector2Add(lhs.to_raylib_vec2(), rhs.to_raylib_vec2()));
}

inline FumoVec2 FumoVec2Negate(FumoVec2 rhs) {
    return to_fumo_vec2(Vector2Negate(rhs.to_raylib_vec2()));
}

inline FumoVec2 FumoVec2Normalize(FumoVec2 lhs) {
    return to_fumo_vec2(Vector2Normalize(lhs.to_raylib_vec2()));
}

inline FumoVec2 FumoVec2Subtract(FumoVec2 lhs, FumoVec2 rhs) {
    return to_fumo_vec2(Vector2Subtract(lhs.to_raylib_vec2(), rhs.to_raylib_vec2()));
}

inline float FumoVec2Length(FumoVec2 v) {
    return Vector2Length(v.to_raylib_vec2());
}

inline FumoVec2 FumoVec2Scale(FumoVec2 lhs, float rhs) {
    return to_fumo_vec2(Vector2Scale(lhs.to_raylib_vec2(), rhs));
}

inline FumoVec2 FumoVec2Divide(FumoVec2 lhs, FumoVec2 rhs) {
    return to_fumo_vec2(Vector2Divide(lhs.to_raylib_vec2(), rhs.to_raylib_vec2()));
}

inline FumoVec2 FumoVec2Multiply(FumoVec2 lhs, FumoVec2 rhs) {
    return to_fumo_vec2(Vector2Multiply(lhs.to_raylib_vec2(), rhs.to_raylib_vec2()));
}

inline FumoVec2 operator+(const FumoVec2& lhs, const FumoVec2& rhs) {
    return FumoVec2Add(lhs, rhs);
}

inline const FumoVec2& operator+=(FumoVec2& lhs, const FumoVec2& rhs) {
    lhs = FumoVec2Add(lhs, rhs);
    return lhs;
}

inline FumoVec2 operator-(const FumoVec2& lhs, const FumoVec2& rhs) {
    return FumoVec2Subtract(lhs, rhs);
}

inline const FumoVec2& operator-=(FumoVec2& lhs, const FumoVec2& rhs) {
    lhs = FumoVec2Subtract(lhs, rhs);
    return lhs;
}

inline FumoVec2 operator*(const FumoVec2& lhs, const float& rhs) {
    return FumoVec2Scale(lhs, rhs);
}

inline const FumoVec2& operator*=(FumoVec2& lhs, const float& rhs) {
    lhs = FumoVec2Scale(lhs, rhs);
    return lhs;
}

inline FumoVec2 operator*(const FumoVec2& lhs, const FumoVec2& rhs) {
    return FumoVec2Multiply(lhs, rhs);
}

inline const FumoVec2& operator*=(FumoVec2& lhs, const FumoVec2& rhs) {
    lhs = FumoVec2Multiply(lhs, rhs);
    return lhs;
}

inline FumoVec2 operator/(const FumoVec2& lhs, const float& rhs) {
    return FumoVec2Scale(lhs, 1.0f / rhs);
}

inline const FumoVec2& operator/=(FumoVec2& lhs, const float& rhs) {
    lhs = FumoVec2Scale(lhs, 1.0f / rhs);
    return lhs;
}

inline FumoVec2 operator/(const FumoVec2& lhs, const FumoVec2& rhs) {
    return FumoVec2Divide(lhs, rhs);
}

inline const FumoVec2& operator/=(FumoVec2& lhs, const FumoVec2& rhs) {
    lhs = FumoVec2Divide(lhs, rhs);
    return lhs;
}

inline bool operator==(const FumoVec2& lhs, const FumoVec2& rhs) {
    return FloatEquals(lhs.x, rhs.x) && FloatEquals(lhs.y, rhs.y);
}

inline bool operator!=(const FumoVec2& lhs, const FumoVec2& rhs) {
    return !FloatEquals(lhs.x, rhs.x) || !FloatEquals(lhs.y, rhs.y);
}

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define FUMO_LIGHTGRAY CLITERAL(FumoColor) {200, 200, 200, 255} // Light Gray
#define FUMO_GRAY CLITERAL(FumoColor) {130, 130, 130, 255} // Gray
#define FUMO_DARKGRAY CLITERAL(FumoColor) {80, 80, 80, 255} // Dark Gray
#define FUMO_YELLOW CLITERAL(FumoColor) {253, 249, 0, 255} // Yellow
#define FUMO_GOLD CLITERAL(FumoColor) {255, 203, 0, 255} // Gold
#define FUMO_ORANGE CLITERAL(FumoColor) {255, 161, 0, 255} // Orange
#define FUMO_PINK CLITERAL(FumoColor) {255, 109, 194, 255} // Pink
#define FUMO_RED CLITERAL(FumoColor) {230, 41, 55, 255} // Red
#define FUMO_MAROON CLITERAL(FumoColor) {190, 33, 55, 255} // Maroon
#define FUMO_GREEN CLITERAL(FumoColor) {0, 228, 48, 255} // Green
#define FUMO_LIME CLITERAL(FumoColor) {0, 158, 47, 255} // Lime
#define FUMO_DARKGREEN CLITERAL(FumoColor) {0, 117, 44, 255} // Dark Green
#define FUMO_SKYBLUE CLITERAL(FumoColor) {102, 191, 255, 255} // Sky Blue
#define FUMO_BLUE CLITERAL(FumoColor) {0, 121, 241, 255} // Blue
#define FUMO_DARKBLUE CLITERAL(FumoColor) {0, 82, 172, 255} // Dark Blue
#define FUMO_PURPLE CLITERAL(FumoColor) {200, 122, 255, 255} // Purple
#define FUMO_VIOLET CLITERAL(FumoColor) {135, 60, 190, 255} // Violet
#define FUMO_DARKPURPLE CLITERAL(FumoColor) {112, 31, 126, 255} // Dark Purple
#define FUMO_BEIGE CLITERAL(FumoColor) {211, 176, 131, 255} // Beige
#define FUMO_BROWN CLITERAL(FumoColor) {127, 106, 79, 255} // Brown
#define FUMO_DARKBROWN CLITERAL(FumoColor) {76, 63, 47, 255} // Dark Brown
#define FUMO_WHITE CLITERAL(FumoColor) {255, 255, 255, 255} // White
#define FUMO_BLACK CLITERAL(FumoColor) {0, 0, 0, 255} // Black
#define FUMO_BLANK CLITERAL(FumoColor) {0, 0, 0, 0} // Blank (Transparent)
#define FUMO_MAGENTA CLITERAL(FumoColor) {255, 0, 255, 255} // Magenta
#define FUMO_RAYWHITE \
    CLITERAL(FumoColor) {245, 245, 245, 255} // My own White (raylib logo)
