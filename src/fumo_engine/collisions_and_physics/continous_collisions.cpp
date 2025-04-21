
#include "point_line_collisions.hpp"

namespace Collisions {

void continuous_rect_collision(PlayerShape& player_shape,
                               Body& player_body,
                               const FumoRect& fumo_rect,
                               const Body& fumo_rect_body,
                               const int& substep_count) {

    // do collision detection with smaller rectangles inside
    // the rectangle we pass in
    // distance between rectangles is at most the radius
}

} // namespace Collisions
