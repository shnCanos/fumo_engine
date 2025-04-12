#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"

#include "fumo_engine/core/global_state.hpp"
extern std::unique_ptr<GlobalState> global;

void weird_debug_print(std::pair<float, FumoVec2> closest_pair,
                       std::pair<float, FumoVec2> bottom_pair,
                       std::pair<float, FumoVec2> left_pair,
                       std::pair<float, FumoVec2> right_pair,
                       std::pair<float, FumoVec2> top_pair) {
    // -------------------------------------------------------------------------------
    // DEBUG
    BeginMode2D(*global->camera);

    if (closest_pair.first == 0.0f) {
    } else if (closest_pair.first == bottom_pair.first) {
        DrawCircleV(bottom_pair.second.to_raylib_vec2(), default_radius, MAGENTA);
    } else if (closest_pair.first == left_pair.first) {
        DrawCircleV(left_pair.second.to_raylib_vec2(), default_radius, ORANGE);
    } else if (closest_pair.first == right_pair.first) {
        DrawCircleV(right_pair.second.to_raylib_vec2(), default_radius, PURPLE);
    } else if (closest_pair.first == top_pair.first) {
        DrawCircleV(top_pair.second.to_raylib_vec2(), default_radius, SKYBLUE);
    }
    EndMode2D();
}

// [[nodiscard]] float PointToLineDistance(const FumoVec2& Point, const FumoVec2&
// LineStart,
//                                         const FumoVec2& LineEnd) {
//     float LineMag;
//     float U;
//     FumoVec2 Intersection;
//
//     LineMag = FumoVec2Distance(LineEnd, LineStart);
//
//     U = (((Point.x - LineStart.y) * (LineEnd.x - LineStart.x)) +
//          ((Point.y - LineStart.y) * (LineEnd.x - LineStart.y))) /
//         (LineMag * LineMag);
//
//     if (U < 0.0f || U > 1.0f)
//         return 0; // closest point does not fall along the line
//
//     Intersection.x = LineStart.x + U * (LineEnd.x - LineStart.x);
//     Intersection.y = LineStart.y + U * (LineEnd.y - LineStart.y);
//
//     // --------------------------------------------------------------------
//     // find the distance from the point to the line intersection
//     // --------------------------------------------------------------------
//
//     float Distance = FumoVec2Distance(Point, Intersection);
//
//     return Distance;
// }
[[nodiscard]] std::pair<float, FumoVec2>
PointToLineDistanceAndIntersection(const FumoVec2& Point,
                                   const FumoVec2& LineStart,
                                   const FumoVec2& LineEnd) {
    // draw a perpendicular line from the point towards the input line
    // Example:   LineStart
    //               |
    //               |
    // (Intersection)*------------ * (Point)
    //               |
    //               |
    //               |
    //            LineEnd
    //
    // If the point falls along the line, return intersection and distance
    // otherwise, return {0, {0.0f, 0.0f}}
    //
    //
    float LineMag;
    FumoVec2 Intersection;

    LineMag = FumoVec2Distance(LineStart, LineEnd);

    FumoVec2 line_vector = LineEnd - LineStart;
    FumoVec2 line_start_to_point = Point - LineStart;
    float dot =
        FumoVec2DotProduct(line_start_to_point, line_vector) / (LineMag * LineMag);

    Intersection.x = LineStart.x + (dot * (LineEnd.x - LineStart.x));
    Intersection.y = LineStart.y + (dot * (LineEnd.y - LineStart.y));

    // --------------------------------------------------------------------
    // check if its along the line
    float buffer = 0.1f;
    float d1 = FumoVec2Distance(Intersection, LineStart);
    float d2 = FumoVec2Distance(Intersection, LineEnd);
    if (d1 + d2 >= LineMag - buffer && d1 + d2 <= LineMag + buffer) {
        float Distance = FumoVec2Distance(Point, Intersection);
        // --------------------------------------------------------------------
        // find the distance from the point to the line intersection
        // --------------------------------------------------------------------
        return {Distance, Intersection};
    }

    return {0, {0.0f, 0.0f}}; // closest point does not fall along the line
}

[[nodiscard]] std::pair<float, FumoVec2>
PointToLineDistanceAndIntersection(const FumoVec2& Point,
                                   const std::pair<FumoVec2, FumoVec2>& line) {
    float LineMag;
    float U;
    FumoVec2 Intersection;
    FumoVec2 LineStart = line.first;
    FumoVec2 LineEnd = line.second;

    LineMag = FumoVec2Distance(LineEnd, LineStart);

    U = (((Point.x - LineStart.y) * (LineEnd.x - LineStart.x))
         + ((Point.y - LineStart.y) * (LineEnd.x - LineStart.y)))
        / (LineMag * LineMag);

    if (U < 0.0f || U > 1.0f)
        return {0.0f, {0.0f, 0.0f}}; // closest point does not fall along the line

    Intersection.x = LineStart.x + U * (LineEnd.x - LineStart.x);
    Intersection.y = LineStart.y + U * (LineEnd.y - LineStart.y);

    // --------------------------------------------------------------------
    // find the distance from the point to the line intersection
    // --------------------------------------------------------------------

    float Distance = FumoVec2Distance(Point, Intersection);

    return {Distance, Intersection};
}

[[nodiscard]] std::pair<float, FumoVec2>
closest_fumo_rect_side(const std::vector<std::pair<float, FumoVec2>>& distances) {
    // used for picking one of 2 fumo_rect sides for solving a collision
    // with a circle
    // we pick the closest side to the circle center

    std::pair<float, FumoVec2> min_pair {6969696, {0.0f, 0.0f}};

    for (const auto& pair : distances) {
        const auto& dist = pair.first;
        if (dist < min_pair.first && dist != 0) {
            min_pair = pair;
        }
    }
    if (min_pair.first == 6969696) {
        return {0.0f, {0.0f, 0.0f}};
    }
    return min_pair;
}

[[nodiscard]] FumoVec2 closest_point(const FumoVec2 target,
                                     const std::vector<FumoVec2>& points) {
    FumoVec2 closest_point {6969, 6969};
    float min_distance {696969};

    for (const auto& point : points) {
        float dist = FumoVec2Distance(target, point);
        if (dist < min_distance) {
            closest_point = point;
            min_distance = dist;
        }
    }
    return closest_point;
}

[[nodiscard]] std::pair<float, FumoVec2>
CircleToRectDistanceAndIntersection(const FumoVec2& circle_center,
                                    const float& radius,
                                    const FumoRect& rect,
                                    const Body& rect_body) {
    FumoVec2 TopLeft = {rect_body.position.x, rect_body.position.y};
    FumoVec2 TopRight = {rect_body.position.x + rect.width, rect_body.position.y};
    FumoVec2 BottomLeft = {rect_body.position.x, rect_body.position.y + rect.height};
    FumoVec2 BottomRight = {rect_body.position.x + rect.width,
                            rect_body.position.y + rect.height};
    std::vector<FumoVec2> points {TopLeft, TopRight, BottomLeft, BottomRight};

    float intersection_distance;
    FumoVec2 closest_corner;
    std::pair<float, FumoVec2> closest_pair = {0.0f, {0.0f, 0.0f}};

    closest_corner = closest_point(circle_center, points);
    intersection_distance = FumoVec2Distance(circle_center, closest_corner);

    if (intersection_distance < radius) {
        // we have a collision with a corner
        // set our corner so it becomes the collision if no side matches
        closest_pair.first = intersection_distance;
        closest_pair.second = closest_corner;
    }
    // -------------------------------------------------------------------------------
    // we have to check the closest sides for collision if none of the corners
    // were a matching collision
    // -------------------------------------------------------------------------------

    std::pair<float, FumoVec2> left_pair, bottom_pair, right_pair, top_pair;

    bottom_pair =
        PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);
    left_pair =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
    right_pair =
        PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
    top_pair = PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);

    // -------------------------------------------------------------------------------
    // weird_debug_print(closest_pair, left_pair, bottom_pair, right_pair, top_pair);

    if (closest_corner == TopLeft) {
        left_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
        top_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
        closest_pair = closest_fumo_rect_side({left_pair, top_pair});

    } else if (closest_corner == TopRight) {
        right_pair =
            PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
        top_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
        closest_pair = closest_fumo_rect_side({right_pair, top_pair});

    } else if (closest_corner == BottomLeft) {
        bottom_pair = PointToLineDistanceAndIntersection(circle_center,
                                                         BottomLeft,
                                                         BottomRight);
        left_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
        closest_pair = closest_fumo_rect_side({bottom_pair, left_pair});

    } else if (closest_corner == BottomRight) {
        right_pair =
            PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
        bottom_pair = PointToLineDistanceAndIntersection(circle_center,
                                                         BottomLeft,
                                                         BottomRight);
        closest_pair = closest_fumo_rect_side({right_pair, bottom_pair});
    }

    return closest_pair;
}

// -------------------------------------------------------------------------------
// Explanation:
// -> Checking agaisnt the capsule's circles
// 1- we take our circle and find the distance between the center of the circle
//  and the closest point to the circle's center,
//  on any of the sides of the fumo_rect.
//  then we take the closest side and pick that as our collision side
//  and move the circle away from that point of intersection
//
// 2- if neither side is valid that means we are colliding with the corner.
//  this means we want to move the circle away from the corner, so we take
//  the closest corner and solve for that
// -------------------------------------------------------------------------------
[[nodiscard]] Collision PlayerToRectCollision(const PlayerShape& player_shape,
                                              const Body& player_body,
                                              const FumoRect& fumo_rect,
                                              const Body& fumo_rect_body) {
    std::pair<float, FumoVec2> closest_dist_intersection = {0.0f, {0.0f, 0.0f}};
    Collision collision = {0.0f, {0.0f, 0.0f}};
    // -------------------------------------------------------------------------------
    // try the bottom circle for collisions
    closest_dist_intersection =
        CircleToRectDistanceAndIntersection(player_shape.bottom_circle_center,
                                            player_shape.radius,
                                            fumo_rect,
                                            fumo_rect_body);

    if (closest_dist_intersection.first != 0.0f) {
        // means any side falls along our center

        // BeginMode2D(*global->camera);
        // DrawCircleV(closest_dist_intersection.second, default_radius, BLUE);
        // DrawLineV(player_body.position, closest_dist_intersection.second, BLUE);
        // EndMode2D();
        if (closest_dist_intersection.first < player_shape.radius) {
            // this means we collided with the bottom circle

            collision.overlap = player_shape.radius - closest_dist_intersection.first;
            collision.push =
                player_shape.bottom_circle_center - closest_dist_intersection.second;

            return collision;
        }
    }
    // -------------------------------------------------------------------------------
    // // try the top circle for collisions
    closest_dist_intersection =
        CircleToRectDistanceAndIntersection(player_shape.top_circle_center,
                                            player_shape.radius,
                                            fumo_rect,
                                            fumo_rect_body);

    if (closest_dist_intersection.first != 0.0f) {
        // this means we collided with the top circle
        // PRINT("COLLISION HAPPENED TOP")
        // BeginMode2D(*global->camera);
        // DrawCircleV(closest_dist_intersection.second, default_radius, BLUE);
        // EndMode2D();
        if (closest_dist_intersection.first < player_shape.radius) {
            collision.overlap = player_shape.radius - closest_dist_intersection.first;
            collision.push =
                player_shape.top_circle_center - closest_dist_intersection.second;
        }
    }
    return collision;
}

[[nodiscard]] Collision PlayerToCircleCollision(const PlayerShape& player_shape,
                                                const Body& player_body,
                                                const Circle& circle_shape,
                                                const Body& circle_body) {
    float radius_sum = player_shape.radius + circle_shape.radius;
    float overlap = 0.0f;
    FumoVec2 push_direction {0.0f, 0.0f};
    // -------------------------------------------------------------------------------
    // bottom circle collision check
    float bottom_distance =
        FumoVec2Distance(circle_body.position, player_shape.bottom_circle_center);
    if (bottom_distance < radius_sum) {
        // collided with the bottom circle
        overlap = radius_sum - bottom_distance;
        push_direction = player_shape.bottom_circle_center - circle_body.position;
        return {overlap, push_direction};
    }
    // -------------------------------------------------------------------------------
    // top circle collision check
    float top_distance =
        FumoVec2Distance(circle_body.position, player_shape.top_circle_center);

    if (top_distance < radius_sum) {
        // collided with top circle
        overlap = radius_sum - top_distance;
        push_direction = player_shape.top_circle_center - circle_body.position;
        return {overlap, push_direction};
    }
    // -------------------------------------------------------------------------------
    // NOTE: remove the capsule sides later but for now i want to test them
    //
    // -------------------------------------------------------------------------------
    // capsule sides collision check
    const auto left_line_distance_pair =
        PointToLineDistanceAndIntersection(circle_body.position,
                                           player_shape.left_line);

    if (left_line_distance_pair.first != 0
        && left_line_distance_pair.first < circle_shape.radius) {
        // collided with left_line
        overlap = circle_shape.radius - left_line_distance_pair.first;
        push_direction = left_line_distance_pair.second - circle_body.position;
        return {overlap, push_direction};
    }

    const auto right_line_distance_pair =
        PointToLineDistanceAndIntersection(circle_body.position,
                                           player_shape.right_line);

    if (right_line_distance_pair.first != 0
        && right_line_distance_pair.first < circle_shape.radius) {
        // collided with right_line
        overlap = circle_shape.radius - right_line_distance_pair.first;
        push_direction = right_line_distance_pair.second - circle_body.position;
        return {overlap, push_direction};
    }

    // -------------------------------------------------------------------------------
    // no collision happened
    return {overlap, push_direction};
}
