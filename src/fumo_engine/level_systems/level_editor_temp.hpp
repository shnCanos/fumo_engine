// #pragma once
//
//
// #include <array>
//
// #include "fumo_engine/core/system_base.hpp"
// #include "fumo_raylib.hpp"
//
// // if we collide with it, we change the screen we are on
// struct ScreenTransitionRect {};
//
// // works the exact same way as an orbit transition:
// // - wait for the screen to finish transitioning before we unfreeze the game
// // - only when we leave the range of the screen transition
// //   do we allow the player to transition again back to the previous screen
//
// struct Level {};
//
// // level object positions are always set by the following:
// // - top left of the screen is {0,0} relative to the objects
// // - when we draw/place them in the world, we add the screen top left
// struct Screen {
//     FumoVec2 screen_position; // screen top left position
// };
//
// // struct OnScreen{};
// using ScreenId = uint64_t;
// constexpr ScreenId LEVEL_COUNT = 64;
//
// struct LevelManager: public System {
//     // sends out a screen transition event
//     // we change camera position and other stuff
//
//     std::array<ScreenId, LEVEL_COUNT> all_level_ids {};
//
//     void sys_call() override {
//         check_for_screen_transition();
//     }
//
//     void check_for_screen_transition();
//
//     void load_level_to_ECS();
// };
//
// // need to somehow associate the objects to each screen/level
// // and it needs to also work with the serialization of levels
// // do we save what level it WAS on? am i overthinking the complexity
// // of properly structuring serialized data in a game?
// // TODO:
// // > look at godot's Screen class and see if that helps with what im writing
//
// struct LevelEditorGUI {
//     void save_level();
//     // ... add new level editor functionality here
//     // or copy it from the current level editor input class
// };
//
