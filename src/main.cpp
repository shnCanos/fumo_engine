#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
std::unique_ptr<GlobalState> global;
void register_all_to_ECS();
int main(void) {
    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");
    global = std::make_unique<GlobalState>();
    global->initialize();

    register_all_to_ECS();
    SetTargetFPS(144);
    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        global->frametime = GetFrameTime();
        global->ECS.run_systems();
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

   return 0;
}
