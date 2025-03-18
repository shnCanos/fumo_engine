#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "raymath.h"

// [[nodiscard]] float PointToLineDistance(const Vector2& Point, const Vector2&
// LineStart,
//                                         const Vector2& LineEnd) {
//     float LineMag;
//     float U;
//     Vector2 Intersection;
//
//     LineMag = Vector2Distance(LineEnd, LineStart);
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
//     float Distance = Vector2Distance(Point, Intersection);
//
//     return Distance;
// }
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(const Vector2& Point, const Vector2& LineStart,
                                   const Vector2& LineEnd) {
    float LineMag;
    float U;
    Vector2 Intersection;

    LineMag = Vector2Distance(LineEnd, LineStart);

    U = (((Point.x - LineStart.y) * (LineEnd.x - LineStart.x)) +
         ((Point.y - LineStart.y) * (LineEnd.x - LineStart.y))) /
        (LineMag * LineMag);

    if (U < 0.0f || U > 1.0f)
        return {0, {0.0f, 0.0f}}; // closest point does not fall along the line

    Intersection.x = LineStart.x + U * (LineEnd.x - LineStart.x);
    Intersection.y = LineStart.y + U * (LineEnd.y - LineStart.y);

    // --------------------------------------------------------------------
    // find the distance from the point to the line intersection
    // --------------------------------------------------------------------

    float Distance = Vector2Distance(Point, Intersection);

    return {Distance, Intersection};
}

[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(const Vector2& Point,
                                   const std::pair<Vector2, Vector2>& line) {
    float LineMag;
    float U;
    Vector2 Intersection;
    Vector2 LineStart = line.first;
    Vector2 LineEnd = line.second;

    LineMag = Vector2Distance(LineEnd, LineStart);

    U = (((Point.x - LineStart.y) * (LineEnd.x - LineStart.x)) +
         ((Point.y - LineStart.y) * (LineEnd.x - LineStart.y))) /
        (LineMag * LineMag);

    if (U < 0.0f || U > 1.0f)
        return {0.0f, {0.0f, 0.0f}}; // closest point does not fall along the line

    Intersection.x = LineStart.x + U * (LineEnd.x - LineStart.x);
    Intersection.y = LineStart.y + U * (LineEnd.y - LineStart.y);

    // --------------------------------------------------------------------
    // find the distance from the point to the line intersection
    // --------------------------------------------------------------------

    float Distance = Vector2Distance(Point, Intersection);

    return {Distance, Intersection};
}

[[nodiscard]] std::pair<float, Vector2>
closest_rectangle_side(const std::vector<std::pair<float, Vector2>>& distances) {

    // used for picking one of 2 rectangle sides for checking agaisnt
    // the circle for collision
    // we pick the closest side to the circle center

    std::pair<float, Vector2> min_pair{6969696, {0.0f, 0.0f}};

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
[[nodiscard]] Vector2 closest_point(const Vector2 target,
                                    const std::vector<Vector2>& points) {

    Vector2 closest_point{6969, 6969};
    float min_distance{696969};

    for (const auto& point : points) {
        float dist = Vector2Distance(target, point);
        if (dist < min_distance) {
            closest_point = point;
            min_distance = dist;
        }
    }
    return closest_point;
}
// [[nodiscard]] Vector2 LineToLineIntersection(const std::pair<Vector2, Vector2>& line1,
//                                              const std::pair<Vector2, Vector2>& line2)
//                                              {
//
//     // calculate the distance to intersection point
//
//     Vector2 intersection{0.0f, 0.0f};
//
//     float uA = ((line2.second.x - line2.first.x) * (line1.first.y - line2.first.y) -
//                 (line2.second.y - line2.first.y) * (line1.first.x - line2.first.x)) /
//                ((line2.second.y - line2.first.y) * (line1.second.x - line1.first.x) -
//                 (line2.second.x - line2.first.x) * (line1.second.y - line1.first.y));
//
//     float uB = ((line1.second.x - line1.first.x) * (line1.first.y - line2.first.y) -
//                 (line1.second.y - line1.first.y) * (line1.first.x - line2.first.x)) /
//                ((line2.second.y - line2.first.y) * (line1.second.x - line1.first.x) -
//                 (line2.second.x - line2.first.x) * (line1.second.y - line1.first.y));
//
//     // if uA and uB are between 0-1, lines are colliding
//     if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
//         intersection.x = line1.first.x + (uA * (line1.second.x - line1.first.x));
//         intersection.y = line1.first.y + (uA * (line1.second.y - line1.first.y));
//     }
//     return intersection;
// }
[[nodiscard]] std::pair<float, Vector2>
CircleToRectDistanceAndIntersection(const Vector2& circle_center, const float& radius,
                                    const Rectangle& rect, const Body& rect_body) {

    Vector2 TopLeft = {rect_body.position.x, rect_body.position.y};
    Vector2 TopRight = {rect_body.position.x + rect.width, rect_body.position.y};
    Vector2 BottomLeft = {rect_body.position.x, rect_body.position.y + rect.height};
    Vector2 BottomRight = {rect_body.position.x + rect.width,
                           rect_body.position.y + rect.height};
    std::vector<Vector2> points{TopLeft, TopRight, BottomLeft, BottomRight};

    float intersection_distance;
    Vector2 closest_corner;
    std::pair<float, Vector2> closest_pair = {0.0f, {0.0f, 0.0f}};

    closest_corner = closest_point(circle_center, points);
    intersection_distance = Vector2Distance(circle_center, closest_corner);
    closest_pair.first = intersection_distance;
    closest_pair.second = closest_corner;

    if (intersection_distance < radius) {
        // we have a collision with a corner
        // return early and check nothing else
        return {intersection_distance, closest_corner};
    }
    // -------------------------------------------------------------------------------
    // we have to check the closest sides for collision if none of the corners
    // were a matching collision
    // -------------------------------------------------------------------------------

    std::pair<float, Vector2> left_pair, bottom_pair, right_pair, top_pair;

    if (closest_corner == TopLeft) {
        left_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
        top_pair = PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
        closest_pair = closest_rectangle_side({left_pair, top_pair});

    } else if (closest_corner == TopRight) {
        right_pair =
            PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
        top_pair = PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
        closest_pair = closest_rectangle_side({right_pair, top_pair});

    } else if (closest_corner == BottomLeft) {
        bottom_pair =
            PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);
        left_pair =
            PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
        closest_pair = closest_rectangle_side({bottom_pair, left_pair});

    } else if (closest_corner == BottomRight) {
        right_pair =
            PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
        bottom_pair =
            PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);
        closest_pair = closest_rectangle_side({right_pair, bottom_pair});
    }

    return closest_pair;
}

[[nodiscard]] Collision PlayerToRectCollision(const PlayerShape& player_shape,
                                              const Body& player_body,
                                              const Rectangle& rectangle,
                                              const Body& rectangle_body) {

    // -------------------------------------------------------------------------------
    // Explanation:
    // -> Checking agaisnt the capsule's circles
    // 1- we take our circle and find the distance between the center of the circle
    //  and the closest point to the circle's center,
    //  on any of the sides of the rectangle.
    //  then we take the closest side and pick that as our collision side
    //  and move the circle away from that point of intersection
    //
    // 2- if neither side is valid that means we are colliding with the corner.
    //  this means we want to move the circle away from the corner, so we take
    //  the closest corner and solve for that
    // -------------------------------------------------------------------------------
    std::pair<float, Vector2> closest_dist_intersection = {0.0f, {0.0f, 0.0f}};
    Collision collision = {0.0f, {0.0f, 0.0f}};
    // -------------------------------------------------------------------------------
    // try the bottom circle for collisions
    closest_dist_intersection = CircleToRectDistanceAndIntersection(
        player_shape.bottom_circle_center, player_shape.radius, rectangle,
        rectangle_body);

    if (closest_dist_intersection.first != 0.0f) {
        // this means we collided with the bottom circle
        collision.overlap = player_shape.radius - closest_dist_intersection.first;
        collision.push =
            player_shape.bottom_circle_center - closest_dist_intersection.second;
        return collision;
    }
    // -------------------------------------------------------------------------------
    // try the top circle for collisions
    closest_dist_intersection = CircleToRectDistanceAndIntersection(
        player_shape.top_circle_center, player_shape.radius, rectangle, rectangle_body);

    if (closest_dist_intersection.first != 0.0f) {
        // this means we collided with the top circle
        collision.overlap = player_shape.radius - closest_dist_intersection.first;
        collision.push =
            player_shape.top_circle_center - closest_dist_intersection.second;
        return collision;
    }
    return collision;
}

[[nodiscard]] Collision PlayerToCircleCollision(const PlayerShape& player_shape,
                                                const Body& player_body,
                                                const Circle& circle_shape,
                                                const Body& circle_body) {

    float radius_sum = player_shape.radius + circle_shape.radius;
    float overlap = 0.0f;
    Vector2 push_direction{0.0f, 0.0f};
    // -------------------------------------------------------------------------------
    // bottom circle collision check
    float bottom_distance =
        Vector2Distance(circle_body.position, player_shape.bottom_circle_center);
    if (bottom_distance < radius_sum) {
        // collided with the bottom circle
        overlap = radius_sum - bottom_distance;
        push_direction = player_shape.bottom_circle_center - circle_body.position;
        return {overlap, push_direction};
    }
    // -------------------------------------------------------------------------------
    // top circle collision check
    float top_distance =
        Vector2Distance(circle_body.position, player_shape.top_circle_center);

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
        PointToLineDistanceAndIntersection(circle_body.position, player_shape.left_line);

    if (left_line_distance_pair.first != 0 &&
        left_line_distance_pair.first < circle_shape.radius) {
        // collided with left_line
        overlap = circle_shape.radius - left_line_distance_pair.first;
        push_direction = left_line_distance_pair.second - circle_body.position;
        return {overlap, push_direction};
    }

    const auto right_line_distance_pair = PointToLineDistanceAndIntersection(
        circle_body.position, player_shape.right_line);

    if (right_line_distance_pair.first != 0 &&
        right_line_distance_pair.first < circle_shape.radius) {
        // collided with right_line
        overlap = circle_shape.radius - right_line_distance_pair.first;
        push_direction = right_line_distance_pair.second - circle_body.position;
        return {overlap, push_direction};
    }

    // -------------------------------------------------------------------------------
    // no collision happened
    return {overlap, push_direction};
}
