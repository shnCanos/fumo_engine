#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_animation_systems.hpp"
#include "objects/components.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

// specific segmentation of loads
void load_player_textures();

void initialize_all_textures() { load_player_textures(); }

void load_player_textures() {
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/The_Dude_Free/Jump.png"),
                      .sprite_sheet_name = "jump",
                      .sprite_frame_count = 5,
                      .base_frame_speed = 2});
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/The_Dude_Free/Idle.png"),
                      .sprite_sheet_name = "idle",
                      .sprite_frame_count = 4,
                      .base_frame_speed = 8,
                      .looping = true});
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/The_Dude_Free/Land.png"),
                      .sprite_sheet_name = "land",
                      .sprite_frame_count = 4,
                      .base_frame_speed = 6});
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/The_Dude_Free/Sprint.png"),
                      .sprite_sheet_name = "sprint",
                      .sprite_frame_count = 8,
                      .base_frame_speed = 5,
                      .looping = true});
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/The_Dude_Free/Walk.png"),
                      .sprite_sheet_name = "walk",
                      .sprite_frame_count = 8,
                      .base_frame_speed = 6,
                      .looping = true});
}

// void func() {
//     // AnimationInfo parameters are optional and we should only touch the
//     // "frame_speed" variable for customizing the animation
//     global->ECS->entity_add_component(global->player_id, AnimationInfo{});
//
//     const auto& animation_player = global->ECS->get_system<AnimationPlayer>();
//
//     animation_player->play(global->ECS->get_component<AnimationInfo>(global->player_id),
//                            "scarfy");
// }
