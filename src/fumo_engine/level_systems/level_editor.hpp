#pragma once
#include "constants.hpp"
#include "fumo_engine/core/system_base.hpp"

// #include "raylib.h"

constexpr uint64_t MAX_SCREENS = 256;

struct ScreenId {
    // tells us what screen an entity belongs to
    // (wrapping an int with type information)
    uint64_t screen_id;
    SERIALIZE(screen_id)
};

struct LevelId {
    // tells us what level an entity belongs to
    uint64_t level_id;
    SERIALIZE(level_id)
};

struct LevelManager {
    // responsible for updating the ScreenId and LevelId on "Ctrl + S"
};

struct ScreenTransitionHandler: System {
    void sys_call() override {
        check_for_screen_transition();
    }

    void check_for_screen_transition();
};

struct Screen {
    FumoVec2 screen_position; // screen top left position
};

struct LevelEditorGUI {
    void save_level();
};

struct ScreenTransitionRect {};
