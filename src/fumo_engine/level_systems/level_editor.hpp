#pragma once
#include "fumo_engine/core/system_base.hpp"

struct ScreenTransitionHandler: System {
    void sys_call() override {
        check_for_screen_transition();
    }

    void check_for_screen_transition();
};




// struct LevelManager {
//     // responsible for updating the ScreenId and LevelId on "Ctrl + S"
// };
