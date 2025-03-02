#include "raylib.h"
#define CUTE_C2_IMPLEMENTATION
#include "constants.hpp"
#include "fumo_engine/global_state.hpp"
//------------------------------------------------------------------------------------
// :Program main entry point
//------------------------------------------------------------------------------------
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

    Camera2D camera = {{0}};
    const auto& player_body = global->ECS->get_component<Body>(global->player_id);
    camera.target = player_body.position;
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();


        global->camera->zoom += ((float)GetMouseWheelMove() * 0.05f);
        // PRINT(camera.target.x)
        // PRINT(camera.target.y)
        // PRINT(player_body.position.x)
        // PRINT(player_body.position.y)

        // BeginMode2D(*global->camera);
        // DrawLine((int)global->camera->target.x, -screenHeight * 10,
        //          (int)global->camera->target.x, screenHeight * 10, GREEN);
        // DrawLine(-screenWidth * 10, (int)global->camera->target.y, screenWidth * 10,
        //          (int)global->camera->target.y, GREEN);
        // EndMode2D();

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
