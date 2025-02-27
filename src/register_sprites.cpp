#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_texture_systems.hpp"
#include "objects/components.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

// specific segmentation of loads
void load_player_textures();

void initialize_all_textures() { load_player_textures(); }

void load_player_textures() {
    global->sprite_manager->register_sprite(
        SpriteSheet2D{.texture_sheet = LoadTexture("assets/scarfy.png"),
                      .sprite_sheet_name = "scarfy",
                      .sprite_frame_count = 6,
                      .base_frame_speed = 6});
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
