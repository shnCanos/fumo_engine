#include "initialization.hpp"

#include "fumo_raylib.hpp"

void Initialization::initialize_window() {
    // TODO: scale the window appropriately based on the monitor
    // we are on, among other things
    // (set up the correct game scale)
    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");
}
