#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_animation_systems.hpp"
#include "objects/components.hpp"
#include "raylib.h"
#include "sprite_and_animation_systems.hpp"

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
    // NOTE:
    // destination.x/y -> where to draw on the screen
    // origin -> where do we center the rectangle itself according to destination.x/y
    // origin is relative to the destination rectangle
    // width and height provide the scaling of our texture

    float sprite_scaling = 2.5f;

    // PRINT("GAMING");
    // debug_print_animation_info(animation_info);


    Rectangle destination = {body.position.x, body.position.y,
                             animation_info.current_region_rect.width * sprite_scaling,
                             animation_info.current_region_rect.height * sprite_scaling};

    Vector2 origin = {destination.width / 2, destination.height / 2};

    DrawTexturePro(sheet_texture, animation_info.current_region_rect, destination,
                   origin, body.rotation, WHITE);
}
