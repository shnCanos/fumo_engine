#include "main_functions.hpp"

#include "fumo_engine/core/global_state.hpp"
#include "fumo_raylib.hpp"

extern std::unique_ptr<GlobalState> global;

DIRECTION opposite_direction(DIRECTION direction) {
    if (direction == DIRECTION::LEFT) {
        return DIRECTION::RIGHT;
    }
    if (direction == DIRECTION::RIGHT) {
        return DIRECTION::LEFT;
    }
    if (direction == DIRECTION::UP) {
        return DIRECTION::DOWN;
    }
    if (direction == DIRECTION::DOWN) {
        return DIRECTION::UP;
    }
    PANIC("UNREACHABLE OR GAVE PREVIOUS_DIRECTION (FIX THIS)");
    std::unreachable();
}

[[nodiscard]] FumoVec2 direction_to_vector(DIRECTION direction) {
    switch (direction) {
        case DIRECTION::LEFT:
            return {-1.0f, 0.0f};
        case DIRECTION::RIGHT:
            return {1.0f, 0.0f};
        case DIRECTION::UP:
            return {0.0f, -1.0f};
        case DIRECTION::DOWN:
            return {0.0f, 1.0f};
        default:
            PRINT("sent to unreachable code NO_DIRECTION");
            std::unreachable();
            break;
    }
}

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

    camera->offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    FumoVec2 diff =
        FumoVec2Subtract(player.position, to_fumo_vec2(camera->target));
    float length = FumoVec2Length(diff);

    float scaling = 8.0f;

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target =
            FumoVec2Add(to_fumo_vec2(camera->target),
                        FumoVec2Scale(diff, speed * global->frametime / length)
                            * scaling)
                .to_raylib_vec2();
    }
}

void debug_player_drawing(const PlayerShape& player_shape,
                          const Body& player_body) {
    // extra visualization code
    BeginMode2D(*global->camera);
    const auto& render = global->ECS->get_component<Render>(global->player_id);
    DrawCircleV(player_shape.bottom_circle_center.to_raylib_vec2(),
                player_shape.radius,
                render.color.to_raylib_color());

    DrawCircleV(player_shape.top_circle_center.to_raylib_vec2(),
                player_shape.radius,
                render.color.to_raylib_color());
    DrawLineEx(player_shape.left_line.first.to_raylib_vec2(),
               player_shape.left_line.second.to_raylib_vec2(),
               4.0f,
               render.color.to_raylib_color());
    DrawLineEx(player_shape.right_line.first.to_raylib_vec2(),
               player_shape.right_line.second.to_raylib_vec2(),
               4.0f,
               render.color.to_raylib_color());

    // double gravity_reach = 300.0f;
    // FumoVec2 normalized_velocity = FumoVec2Normalize(player_body.velocity);
    // FumoVec2 line_end = player_body.position + normalized_velocity * gravity_reach;
    // if (player_state.jumping) {
    //     line_end =
    //         player_body.position - (player_body.gravity_direction) * gravity_reach;
    // }
    // DrawLineV(player_body.position, line_end, YELLOW);
    EndMode2D();
}
