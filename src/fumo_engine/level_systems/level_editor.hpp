#pragma once
#include "fumo_engine/core/system_base.hpp"

// #include "raylib.h"

struct ScreenId {
    // tells us what screen an entity belongs to
    // (wrapping an int with type information)
    uint64_t screen_id;
};

struct LevelId {
    // tells us what level an entity belongs to
    uint64_t level_id;
};

// component mask: Any entity with LevelId component
// (which is all entities in the engine)
// (we might have entities that are outside the scope of the engine,
// so the component mask targets specifically "game" entities)
struct LevelSerializer: public System {
    void sys_call() override {
        // never gets called (we just need to get updated by the ECS)
    };
    // basically the "Ctrl + S" of the engine
    // updates the serialized data when called
    void serialize_levels();
    // goes through the serialized data and assigns ScreenId and LevelId
    // based on where stuff was stored (directory and file name)
    void deserialize_levels();
};

struct ScreenHandler: System {
    void sys_call() override {
        check_for_screen_transition();
    }

    void check_for_screen_transition();
};

// struct Screen {
//     Vector2 screen_position; // screen top left position
// };
//
// struct LevelEditorGUI {
//     void save_level();
// };
//
// struct ScreenTransitionRect {};
