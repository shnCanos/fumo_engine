#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"

std::unique_ptr<GlobalState> global;

void register_all_to_ECS();
void initialize_all_textures();
void debug_spawn_level_objects();

int main(void) {
    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    global = std::make_unique<GlobalState>();
    global->initialize();
    //--------------------------------------------------------------------------------------
    // must be done before global->setup_game_state();
    initialize_all_textures();
    register_all_to_ECS();
    //--------------------------------------------------------------------------------------

    global->setup_game_state();

    SetTargetFPS(60);
    int count = 0;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        global->frametime = GetFrameTime();

        global->ECS->run_systems();
        // events are handled after all systems are ran
        global->handle_events();

        // here because we start with no planets right now (remove whem we make levels)
        if (!count) [[unlikely]] {
            count++;
            debug_spawn_level_objects();
        }

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
