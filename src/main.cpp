#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_texture_systems.hpp"
//------------------------------------------------------------------------------------
// :Program main entry point
//------------------------------------------------------------------------------------
std::unique_ptr<GlobalState> global;
std::unique_ptr<SpriteManager> sprite_manager;
// NOTE: NEW GLOBAL VARIABLE ADDED

void register_all_to_ECS();

int main(void) {

    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    global = std::make_unique<GlobalState>();
    global->initialize();

    register_all_to_ECS();

    global->setup_game_state();

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        global->frametime = GetFrameTime();
        global->ECS->run_systems();
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    // unload textures before closing the OpenGL context
    global->destroy_and_unload_game();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
