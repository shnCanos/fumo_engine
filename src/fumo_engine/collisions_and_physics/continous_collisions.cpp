#include "fumo_engine/collisions_and_physics/collision_functions.hpp"

namespace Collisions {

[[nodiscard]] std::vector<RectBodyPair>
calculate_sub_rectangles(const FumoRect& fumo_rect,
                         const Body& fumo_rect_body,
                         const int& substep_count,
                         const int& max_sub_distance) {

    constexpr float leeway = 0.6f; // makes detection more consistent
    const float actual_subdistance = max_sub_distance - leeway;

    std::vector<RectBodyPair> sub_rectangles {};
    RectBodyPair current_pair = {.fumo_rect = fumo_rect,
                                 .body = fumo_rect_body};
    // FIXME: remove this update later when not debugging
    //
    // UpdateCameraCenterSmoothFollow(
    //     global->camera.get(),
    //     global->ECS->get_component<Body>(global->player_id));
    // BeginMode2D(*global->camera);
    // FumoDrawRectV(fumo_rect_body.position,
    //               {fumo_rect.width, fumo_rect.height},
    //               FUMO_RED);

    for (int i = 0; i < substep_count; i++) {
        current_pair.body.position.x += actual_subdistance;
        current_pair.body.position.y += actual_subdistance;
        current_pair.fumo_rect.width -= actual_subdistance * 2.0f;
        current_pair.fumo_rect.height -= actual_subdistance * 2.0f;
        if (current_pair.fumo_rect.width <= 0
            || current_pair.fumo_rect.height <= 0) {
            // EndMode2D();
            return sub_rectangles;
        }
        // FumoDrawRectV(
        //     current_pair.body.position,
        //     {current_pair.fumo_rect.width, current_pair.fumo_rect.height},
        //     all_colors[i]);

        sub_rectangles.push_back(current_pair);
    }
    // EndMode2D();
    return sub_rectangles;
}

// do collision detection with smaller rectangles inside
// the rectangle we pass in.
// the distance between sub_rectangles is, at most, the radius of
// the player_capsule circles (prevents player from getting stuck
// inside the rectangle)
[[nodiscard]] Collision
continuous_rect_collision_solving(Capsule& player_capsule,
                                  Body& player_body,
                                  const FumoRect& fumo_rect,
                                  const Body& fumo_rect_body,
                                  const int& substep_count) {
    // distinguish colliding from the left, right, top and bottom
    // (use the capsule sides for detecting if its from left or right)

    const auto& sub_rectangles =
        calculate_sub_rectangles(fumo_rect,
                                 fumo_rect_body,
                                 substep_count,
                                 player_capsule.radius - 5.0f);

    for (const auto& rect_body_pair : sub_rectangles) {
        capsule_to_rect_collision_solving(player_capsule,
                                          player_body,
                                          rect_body_pair.fumo_rect,
                                          rect_body_pair.body);
    }
    // after pushing the player to the outer rectangle
    // (if the player was inside the rectangle)
    // we only need to return whether or not we collided with the outer
    // rectangle like before to decide if there was a collision
    return capsule_to_rect_collision_solving(player_capsule,
                                             player_body,
                                             fumo_rect,
                                             fumo_rect_body);
}

void calculate_collided_region(Collision& collision,
                               const Capsule& player_capsule) {
    // TODO: add all 8 points of the capsule (for more complete collision info)

    // BeginMode2D(*global->camera);
    // FumoDrawCircleV(collision.intersection_point, 10.0f, FUMO_GREEN);
    // EndMode2D();
    FumoVec2 closest = closest_point(collision.intersection_point,
                                     {player_capsule.left_line.start,
                                      player_capsule.left_line.end,
                                      player_capsule.right_line.start,
                                      player_capsule.right_line.end,
                                      player_capsule.middle_line.start,
                                      player_capsule.middle_line.end});

    collision.collided_capsule_side =
        (closest == player_capsule.left_line.start
         || closest == player_capsule.left_line.end)
        ? DIRECTION::LEFT
        : (closest == player_capsule.right_line.start
           || closest == player_capsule.right_line.end)
        ? DIRECTION::RIGHT
        : closest == player_capsule.middle_line.start ? DIRECTION::DOWN
        : closest == player_capsule.middle_line.end   ? DIRECTION::UP
                                                      : DIRECTION::NO_DIRECTION;
}

} // namespace Collisions
