#include "main_functions.hpp"
#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

// bool IsKeyStillDown(int key) {
//     bool still_down = false;
//
//     if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
//     {
//         if ((CORE.Input.Keyboard.previousKeyState[key] == 1) && (CORE.Input.Keyboard.currentKeyState[key] == 1)) still_down = true;
//     }
//
//     return still_down;
//
// }
Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
    Vector2 result = {v.x / scale, v.y / scale};
    return result;
}

void debug_print_animation_info(const AnimationInfo& animation_info) {
    PRINT(animation_info.current_region_rect.x);
    PRINT(animation_info.current_region_rect.y);
    // PRINT(animation_info.current_region_rect.width);
    // PRINT(animation_info.current_region_rect.height);
    PRINT(animation_info.current_sheet_name);
    PRINT(animation_info.frame_progress);
    std::cerr << '\n';
}

void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player) {
    float minSpeed = 30;
    float minEffectLength = 10;
    float fractionSpeed = 0.8f;

    camera->offset = (Vector2) {screenWidth / 2.0f, screenHeight / 2.0f};
    Vector2 diff = Vector2Subtract(player.position, camera->target);
    float length = Vector2Length(diff);

    float scaling = 8.0f;

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target = Vector2Add(
            camera->target,
            Vector2Scale(diff, speed * global->frametime / length) * scaling
        );
    }
}

void debug_player_drawing(const PlayerShape& player_shape, const Body& player_body) {
    // extra visualization code
    BeginMode2D(*global->camera);
    const auto& render = global->ECS->get_component<Render>(global->player_id);
    DrawCircleV(player_shape.bottom_circle_center, player_shape.radius, render.color);
    DrawCircleV(player_shape.top_circle_center, player_shape.radius, render.color);

    // double gravity_reach = 300.0f;
    // Vector2 normalized_velocity = Vector2Normalize(player_body.velocity);
    // Vector2 line_end = player_body.position + normalized_velocity * gravity_reach;
    // if (player_state.jumping) {
    //     line_end =
    //         player_body.position - (player_body.gravity_direction) * gravity_reach;
    // }
    // DrawLineV(player_body.position, line_end, YELLOW);
    EndMode2D();
}
