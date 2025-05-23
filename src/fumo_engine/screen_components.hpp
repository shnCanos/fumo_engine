#pragma once

#include <cstdint>

#include "fumo_raylib.hpp"
#include "fumo_serialize_macros.hpp"
constexpr uint64_t MAX_SCREENS = 256;

struct Screen {
    // tells us what screen an entity belongs to
    // (wrapping an int with type information)
    uint64_t screen_id {0};
    FumoVec2 screen_position {0.0f, 0.0f}; // screen top left position
    SERIALIZE(screen_id, screen_position);
};

struct LevelId {
    // tells us what level an entity belongs to
    uint64_t level_id;
    SERIALIZE(level_id)
};

struct ScreenTransitionRect {
    FumoRect transition_rect;
    Screen previous_screen;
    Screen next_screen;
    SERIALIZE(transition_rect, previous_screen, next_screen);
    void draw(const FumoColor& color, const FumoVec2& position) const;
};
