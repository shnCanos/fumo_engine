#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_texture_systems.hpp"
#include "objects/components.hpp"

extern std::unique_ptr<GlobalState> global;

void GlobalSpriteHandler::global_texture_load() { load_player_textures(); }

void GlobalSpriteHandler::load_player_textures() {
    Texture2D player_texture = LoadTexture("assets/scarfy.png");

    // global->ECS->entity_add_component(
    //     global->player_id,
    //     SpriteSheet2D{player_texture,
    //                   "scarfy.png",
    //                   {(float)player_texture.width, (float)player_texture.height}});
    //
    // Sprite2DAnimations player_animations{};

    // player_animations.add_animation("scarfy.png");
    //
    // global->ECS->entity_add_component(global->player_id, player_animations);
}

void AnimationPlayer::play(AnimationInfo animation_info,
                           std::string_view animation_name) {
    animation_info.frame_progress = 0;
    animation_info.sheet_rect_vector.erase(animation_info.sheet_rect_vector.begin());
}
void AnimationPlayer::pause(AnimationInfo animation_info) {}
