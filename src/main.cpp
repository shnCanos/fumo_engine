#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/serialization/fumo_serializer.hpp"
#include "initialization.hpp"
#include "raylib.h"

std::unique_ptr<FumoEngine> fumo_engine;
#define XMACRO(Type) EntityQuery Type##_query {};
ALL_COMPONENTS_X_MACRO()
#undef XMACRO

void debug_spawn_level_objects();

int main(void) {
    int count = 0;

    Initialization::initialize_window();

    //--------------------------------------------------------------------------------------
    fumo_engine = std::make_unique<FumoEngine>();
    fumo_engine->initialize();
    //--------------------------------------------------------------------------------------
    // must be done before fumo_engine->setup_game_state();
    Initialization::initialize_all_textures();
    // all components MUST be added to ALL_COMPONENTS_X_MACRO()
    // to be registered into fumo_engine
    Initialization::register_all_to_fumo_engine();
    //--------------------------------------------------------------------------------------
    fumo_engine->setup_game_state();
    FumoSerializer::deserialize_levels();
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        fumo_engine->frametime = GetFrameTime();

        fumo_engine->ECS->run_systems();
        // events are handled after all systems are ran
        fumo_engine->handle_events();

        // here because we start with no planets right now (remove when we make levels)
        // if (!count) [[unlikely]] {
        //     count++;
        //     debug_spawn_level_objects();
        // }
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    // unload textures before closing the OpenGL context
    // Close window and OpenGL context
    fumo_engine->destroy_and_unload_game();
    //--------------------------------------------------------------------------------------

    return 0;
}
