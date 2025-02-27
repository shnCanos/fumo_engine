#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_animation_systems.hpp"
#include "objects/components.hpp"
#include "sprite_and_animation_systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

void AnimationRenderer::draw_animations() {
    const auto& animation_player = global->ECS->get_system<AnimationPlayer>();
    for (auto entity_id : sys_entities) {
        const auto& animation_info =
            global->ECS->get_component<AnimationInfo>(entity_id);

        const auto& sheet_texture = global->sprite_manager->get_sprite_texture(
            animation_info.current_sheet_name);

        const auto& entity_body = global->ECS->get_component<Body>(entity_id);

        draw_animation(animation_info, sheet_texture, entity_body);
    }
}

void AnimationRenderer::draw_animation(const AnimationInfo& animation_info,
                                       const Texture2D& sheet_texture,
                                       const Body& body) {
    DrawTextureRec(sheet_texture, animation_info.current_region_rect, body.position,
                   WHITE);
}
