#include "fumo_engine/core/global_state.hpp"
#include "objects/factory_systems.hpp"
#include "raylib.h"

std::unique_ptr<GlobalState> global;

void register_all_to_ECS();
void initialize_all_textures();
void debug_spawn_level_objects();

int main(void) {

    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    global = std::make_unique<GlobalState>();
    global->initialize();

    initialize_all_textures();
    register_all_to_ECS();

    global->setup_game_state();

    SetTargetFPS(60);
    int count = 0;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        global->camera->zoom += ((float)GetMouseWheelMove() * 0.05f);

        global->frametime = GetFrameTime();

        global->ECS->run_systems();

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

void debug_spawn_level_objects() {
    // FIXME: the player is currently not transitioning from a circle planet to a rect
    // planet. only in this specific case does it stop working

    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    EntityId id_planet;
    Vector2 starter_position = {screenCenter.x - 150.0f, screenCenter.y + 250.0f};
    {
        id_planet = planet_factory->create_rect_planet(starter_position);

        auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);
        player_shape.player_owning_field = id_planet;

        Rectangle& rect_planet = global->ECS->get_component<Rectangle>(id_planet);
        rect_planet.width = rect_planet.width * 7;

        ParallelGravityField& rect_field =
            global->ECS->get_component<ParallelGravityField>(id_planet);
        rect_field.field_rectangle.width = rect_planet.width;
    }
    {
        starter_position.y -= 850.0f;

        id_planet = planet_factory->create_rect_planet(starter_position);

        Rectangle& rect_planet = global->ECS->get_component<Rectangle>(id_planet);
        rect_planet.width = rect_planet.width * 7;

        ParallelGravityField& rect_field =
            global->ECS->get_component<ParallelGravityField>(id_planet);
        rect_field.field_rectangle.width = rect_planet.width;
        rect_field.field_rectangle.y += rect_planet.height * 2;
        rect_field.gravity_direction = {0.0f, -1.0f};
    }
}
