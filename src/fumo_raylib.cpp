#include "fumo_raylib.hpp"

void FumoDrawCircleV(FumoVec2 center, float radius, FumoColor color) {
    DrawCircleV(center.to_raylib_vec2(), radius, color.to_raylib_color());
}

void FumoDrawLineV(FumoVec2 StartPos, FumoVec2 EndPos, FumoColor color) {
    DrawLineV(StartPos.to_raylib_vec2(),
              EndPos.to_raylib_vec2(),
              color.to_raylib_color());
}

void FumoDrawLineEx(FumoVec2 StartPos,
                    FumoVec2 EndPos,
                    float thick,
                    FumoColor color) {
    DrawLineEx(StartPos.to_raylib_vec2(),
               EndPos.to_raylib_vec2(),
               thick,
               color.to_raylib_color());
}

void FumoDrawRectV(FumoVec2 position, FumoVec2 size, FumoColor color) {
    DrawRectangleV(position.to_raylib_vec2(),
                   size.to_raylib_vec2(),
                   color.to_raylib_color());
}

void FumoDrawRect(FumoRect rect, FumoColor color) {
    DrawRectangleRec(rect.to_raylib_rect(), color.to_raylib_color());
}

void print_direction(DIRECTION direction) {
    switch (direction) {
        case DIRECTION::LEFT:
            PRINT_NO_NAME("LEFT");
            break;
        case DIRECTION::RIGHT:
            PRINT_NO_NAME("RIGHT");
            break;
        case DIRECTION::UP:
            PRINT_NO_NAME("UP");
            break;
        case DIRECTION::DOWN:
            PRINT_NO_NAME("DOWN");
            break;
        case DIRECTION::UP_RIGHT:
            PRINT_NO_NAME("UP_RIGHT");
            break;
        case DIRECTION::UP_LEFT:
            PRINT_NO_NAME("UP_LEFT");
            break;
        case DIRECTION::DOWN_LEFT:
            PRINT_NO_NAME("DOWN_LEFT");
            break;
        case DIRECTION::DOWN_RIGHT:
            PRINT_NO_NAME("DOWN_RIGHT");
            break;
        case DIRECTION::NO_DIRECTION:
            PRINT_NO_NAME("NO_DIRECTION");
            break;
    }
}

[[nodiscard]] DIRECTION opposite_direction(DIRECTION direction) {
    // TODO: complete this function with the new directions

    DIRECTION result = direction == DIRECTION::LEFT ? DIRECTION::RIGHT
        : direction == DIRECTION::RIGHT             ? DIRECTION::LEFT
        : direction == DIRECTION::UP                ? DIRECTION::DOWN
        : direction == DIRECTION::DOWN              ? DIRECTION::UP
        : direction == DIRECTION::UP_RIGHT          ? DIRECTION::UP_LEFT
        : direction == DIRECTION::UP_LEFT           ? DIRECTION::UP_RIGHT
        : direction == DIRECTION::DOWN_LEFT         ? DIRECTION::DOWN_RIGHT
        : direction == DIRECTION::DOWN_RIGHT        ? DIRECTION::DOWN_LEFT
        : direction == DIRECTION::NO_DIRECTION      ? DIRECTION::NO_DIRECTION
                                                    : DIRECTION::NO_DIRECTION;
    if (result == DIRECTION::NO_DIRECTION)
        PANIC("UNREACHABLE OR GAVE PREVIOUS_DIRECTION (FIX THIS)");
    return result;
    // PANIC("UNREACHABLE OR GAVE PREVIOUS_DIRECTION (FIX THIS)");
    // std::unreachable();
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
        case DIRECTION::UP_RIGHT:
            return {1.0f, -1.0f};
        case DIRECTION::UP_LEFT:
            return {-1.0f, -1.0f};
        case DIRECTION::DOWN_LEFT:
            return {-1.0f, 1.0f};
        case DIRECTION::DOWN_RIGHT:
            return {1.0f, 1.0f};
        case DIRECTION::NO_DIRECTION:
        default:
            return {0.0f, 0.0f};
            PANIC("sent to unreachable code NO_DIRECTION");
            std::unreachable();
            break;
    }
}

[[nodiscard]] DIRECTION vector_to_direction(const FumoVec2& vector) {
    DIRECTION result =
        (vector == FumoVec2 {-1.0f, 0.0f}        ? DIRECTION::LEFT
             : vector == FumoVec2 {1.0f, 0.0f}   ? DIRECTION::RIGHT
             : vector == FumoVec2 {0.0f, -1.0f}  ? DIRECTION::UP
             : vector == FumoVec2 {0.0f, 1.0f}   ? DIRECTION::DOWN
             : vector == FumoVec2 {1.0f, -1.0f}  ? DIRECTION::UP_RIGHT
             : vector == FumoVec2 {-1.0f, -1.0f} ? DIRECTION::UP_LEFT
             : vector == FumoVec2 {-1.0f, 1.0f}  ? DIRECTION::DOWN_LEFT
             : vector == FumoVec2 {1.0f, 1.0f}   ? DIRECTION::DOWN_RIGHT
                                                 : DIRECTION::NO_DIRECTION);
    if (result == DIRECTION::NO_DIRECTION)
        PANIC("UNREACHABLE OR GAVE PREVIOUS_DIRECTION (FIX THIS)");
    return result;
}

[[nodiscard]] FumoVec2 FumoVec2Snap8Directions(FumoVec2 v1) {
    DIRECTION direction = static_cast<DIRECTION>(
        (((int)std::round(std::atan2(v1.y, v1.x) / (2 * PI / 8))) + 8) % 8);
    return direction_to_vector(direction);
    // float angle = std::atan2f(v1.y, v1.x) * RAD2DEG;
    // angle = std::round(angle / (360.0f) * directions) / directions * (360.0f);
    // return FumoVec2Rotate({1, 0}, angle) * FumoVec2Length(v1);
}

[[nodiscard]] FumoVec2 _internal_4_directions_to_vector(DIRECTION direction) {
    // hardcoding to make logic easy
    switch (direction) {
        case DIRECTION::RIGHT:
            return {1.0f, 0.0f};
        case DIRECTION::DOWN_RIGHT:
            return {0.0f, 1.0f};
        case DIRECTION::DOWN:
            return {-1.0f, 0.0f};
        case DIRECTION::DOWN_LEFT:
            return {0.0f, -1.0f};
        default:
            PANIC("sent to unreachable code NO_DIRECTION");
            std::unreachable();
            return {0.0f, 0.0f};
    }
}

[[nodiscard]] FumoVec2 FumoVec2Snap4Directions(FumoVec2 v1) {
    DIRECTION direction = static_cast<DIRECTION>(
        (((int)std::round(std::atan2(v1.y, v1.x) / (2 * PI / 4))) + 4) % 4);
    return _internal_4_directions_to_vector(direction);
    // float angle = std::atan2f(v1.y, v1.x) * RAD2DEG;
    // angle = std::round(angle / (360.0f) * directions) / directions * (360.0f);
    // return FumoVec2Rotate({1, 0}, angle) * FumoVec2Length(v1);
}
