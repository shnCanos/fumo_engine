#include "fumo_engine/collisions_and_physics/collision_functions.hpp"

namespace Collisions {

[[nodiscard]] bool LineToRectCollided(const Line& line,
                                      const FumoRect& rect,
                                      const Body& rect_body) {

    FumoVec2 TopLeft = {rect_body.position.x, rect_body.position.y};
    FumoVec2 TopRight = {rect_body.position.x + rect.width,
                         rect_body.position.y};
    FumoVec2 BottomLeft = {rect_body.position.x,
                           rect_body.position.y + rect.height};
    FumoVec2 BottomRight = {rect_body.position.x + rect.width,
                            rect_body.position.y + rect.height};

    const Line left_side {TopLeft, BottomLeft},
        right_side {TopRight, BottomRight}, top_side {TopLeft, TopRight},
        bottom_side {BottomLeft, BottomRight};

    return (LineToLineCollided(line, left_side)
            || LineToLineCollided(line, right_side)
            || LineToLineCollided(line, top_side)
            || LineToLineCollided(line, bottom_side));
}

#define LINE_TO_SIDE_COLLISION(line, side) \
    do { \
        Collision collision = LineToLineCollision(line, side); \
        collision.distance = \
            FumoVec2Distance(line.start, collision.intersection_point); \
        if (collision.collided \
            && (collision.distance < closest_collision.distance \
                || closest_collision.distance == 0.0f)) { \
            collision.push_direction = \
                FumoVec2Normalize(line.start - collision.intersection_point); \
            collision.overlap = \
                FumoVec2Distance(line.start, line.end) - collision.distance; \
            collision.collided_shape = SHAPE::Rectangle; \
            collision.normal = \
                FumoVec2Snap4Directions(collision.push_direction); \
            closest_collision = collision; \
        } \
    } while (0)

[[nodiscard]] const Collision LineToRectCollision(const Line& line,
                                                  const FumoRect& rect,
                                                  const Body& rect_body) {
    Collision closest_collision;

    FumoVec2 TopLeft = {rect_body.position.x, rect_body.position.y};
    FumoVec2 TopRight = {rect_body.position.x + rect.width,
                         rect_body.position.y};
    FumoVec2 BottomLeft = {rect_body.position.x,
                           rect_body.position.y + rect.height};
    FumoVec2 BottomRight = {rect_body.position.x + rect.width,
                            rect_body.position.y + rect.height};

    const Line left_side {TopLeft, BottomLeft},
        right_side {TopRight, BottomRight}, top_side {TopLeft, TopRight},
        bottom_side {BottomLeft, BottomRight};

    // NOTE: if we add rotations, you need to replace FumoVec2Snap4Directions()
    // with actually finding the normal to the surface

    // PRINT(collision.normal.x);
    // PRINT(collision.normal.y)
    // print_direction(vector_to_direction(collision.normal));

    LINE_TO_SIDE_COLLISION(line, left_side);
    LINE_TO_SIDE_COLLISION(line, right_side);
    LINE_TO_SIDE_COLLISION(line, top_side);
    LINE_TO_SIDE_COLLISION(line, bottom_side);

    return closest_collision;
}

} // namespace Collisions
