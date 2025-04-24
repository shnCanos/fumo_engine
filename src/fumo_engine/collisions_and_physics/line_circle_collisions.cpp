// Find the points of intersection.
#include <cmath>

#include "fumo_engine/collisions_and_physics/collision_functions.hpp"

namespace Collisions {

int FindLineCircleIntersections(float cx,
                                float cy,
                                float radius,
                                FumoVec2 point1,
                                FumoVec2 point2,
                                FumoVec2& intersection1,
                                FumoVec2& intersection2) {
    float dx, dy, A, B, C, det, t;

    dx = point2.x - point1.x;
    dy = point2.y - point1.y;

    A = dx * dx + dy * dy;
    B = 2 * (dx * (point1.x - cx) + dy * (point1.y - cy));
    C = (point1.x - cx) * (point1.x - cx) + (point1.y - cy) * (point1.y - cy)
        - radius * radius;

    det = B * B - 4 * A * C;
    if ((A <= 0.0000001) || (det < 0)) {
        // No real solutions.
        intersection1 = {0.0f, 0.0f};
        intersection2 = {0.0f, 0.0f};
        return 0;
    } else if (det == 0) {
        // One solution.
        t = -B / (2 * A);
        intersection1 = {point1.x + t * dx, point1.y + t * dy};
        intersection2 = {0.0f, 0.0f};
        return 1;
    } else {
        // Two solutions.
        t = (float)((-B + std::sqrtf(det)) / (2 * A));
        intersection1 = {point1.x + t * dx, point1.y + t * dy};
        t = (float)((-B - std::sqrtf(det)) / (2 * A));
        intersection2 = {point1.x + t * dx, point1.y + t * dy};
        return 2;
    }
}

//cx,cy is center point of the circle
[[nodiscard]] FumoVec2 ClosestPointLineIntersection(const FumoVec2& Point,
                                                    const Line& line,
                                                    const float& radius) {
    FumoVec2 intersection1;
    FumoVec2 intersection2;
    int intersections = FindLineCircleIntersections(Point.x,
                                                    Point.y,
                                                    radius,
                                                    line.start,
                                                    line.end,
                                                    intersection1,
                                                    intersection2);

    if (intersections == 1) return intersection1; // one intersection

    if (intersections == 2) {
        double dist1 = FumoVec2Distance(intersection1, line.start);
        double dist2 = FumoVec2Distance(intersection2, line.start);

        if (dist1 < dist2) return intersection1;
        return intersection2;
    }

    return {0.0f, 0.0f}; // no intersections at all
}


[[nodiscard]] const Collision LineToCircleCollision(const Line& line,
                                                    const Circle& circle_shape,
                                                    const Body& circle_body) {

    Collision collision {};

    const FumoVec2 closest_intersection =
        ClosestPointLineIntersection(circle_body.position,
                                     line,
                                     circle_shape.radius);
    if (closest_intersection != FumoVec2 {0.0f, 0.0f}) {
        float buffer = 0.1f;
        // distance from each end of the line should add up to the length
        // if we are on the line
        float LineLength = FumoVec2Distance(line.start, line.end);
        float d1 = FumoVec2Distance(closest_intersection, line.start);
        float d2 = FumoVec2Distance(closest_intersection, line.end);

        if (d1 + d2 >= LineLength - buffer && d1 + d2 <= LineLength + buffer) {
            collision.intersection_point = closest_intersection;
            collision.normal_or_push =
                FumoVec2Normalize(closest_intersection - circle_body.position);
            collision.normal = collision.normal_or_push;
            collision.collided = true;
            collision.distance =
                FumoVec2Distance(line.start, closest_intersection);
            collision.collided_shape = SHAPE::Circle;
        }
        // not all collision information is guaranteed to be filled
    }
    return collision;
}

} // namespace Collisions
