#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/serialization/fumo_serializer.hpp"
#include "raylib.h"

std::unique_ptr<FumoEngine> fumo_engine;
#define XMACRO(Type) EntityQuery Type##_query {};
ALL_COMPONENTS_X_MACRO()
#undef XMACRO

void register_all_to_ECS();
void initialize_all_textures();
void debug_spawn_level_objects();

int main(void) {
    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    fumo_engine = std::make_unique<FumoEngine>();
    fumo_engine->initialize();
    //--------------------------------------------------------------------------------------
    // must be done before fumo_engine->setup_game_state();
    initialize_all_textures();
    // all components must be added to ALL_COMPONENTS_X_MACRO()
    // to be registered into fumo_engine
    register_all_to_ECS();
    //--------------------------------------------------------------------------------------

    fumo_engine->setup_game_state();

    SetTargetFPS(60);
    int count = 0;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        fumo_engine->frametime = GetFrameTime();

        fumo_engine->ECS->run_systems();
        // events are handled after all systems are ran
        fumo_engine->handle_events();

        // here because we start with no planets right now (remove when we make levels)
        if (!count) [[unlikely]] {
            count++;
            // debug_spawn_level_objects();
            const auto& level_serializer =
                fumo_engine->ECS->get_system<LevelSerializer>();
            level_serializer->deserialize_levels();
        }

        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    // unload textures before closing the OpenGL context
    fumo_engine->destroy_and_unload_game();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
