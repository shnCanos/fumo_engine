
#include "point_line_collisions.hpp"

namespace Collisions {

struct RectBodyPair {
    FumoRect fumo_rect;
    Body body;
};

void continuous_rect_collision(PlayerShape& player_shape,
                               Body& player_body,
                               const FumoRect& fumo_rect,
                               const Body& fumo_rect_body,
                               const int& substep_count) {
    // do collision detection with smaller rectangles inside
    // the rectangle we pass in.
    // the distance between sub_rectangles is, at most, the radius of
    // the player_shape circles (prevents player from getting stuck
    // inside the rectangle)



}

// [[nodiscard]] std::vector<RectBodyPair>
// calculate_sub_rectangles(const FumoRect& fumo_rect,
//                          const Body& fumo_rect_body,
//                          const int& substep_count,
//                          const int& max_sub_distance) {}

} // namespace Collisions
