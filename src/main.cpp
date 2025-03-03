#include "raylib.h"
#define CUTE_C2_IMPLEMENTATION
#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
// #include "clay_stuff.c"


std::unique_ptr<GlobalState> global;

void register_all_to_ECS();
void initialize_all_textures();

int main(void) {

    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    global = std::make_unique<GlobalState>();
    global->initialize();

    initialize_all_textures();
    register_all_to_ECS();

    global->setup_game_state();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        global->camera->zoom += ((float)GetMouseWheelMove() * 0.05f);

        global->frametime = GetFrameTime();

        global->ECS->run_systems();

        DrawFPS(10, 10);
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    // unload textures before closing the OpenGL context
    global->destroy_and_unload_game();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
