#include "main_functions.hpp"
Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
    Vector2 result = {v.x / scale, v.y / scale};
    return result;
}
[[nodiscard]] float PointToLineDistance(Vector2& Point, Vector2& LineStart,
                                        Vector2& LineEnd) {
    float LineMag;
    float U;
    Vector2 Intersection;

    LineMag = Vector2Distance(LineEnd, LineStart);

    U = (((Point.x - LineStart.y) * (LineEnd.x - LineStart.x)) +
         ((Point.y - LineStart.y) * (LineEnd.x - LineStart.y))) /
        (LineMag * LineMag);

    if (U < 0.0f || U > 1.0f)
        return 0; // closest point does not fall along the line

    Intersection.x = LineStart.x + U * (LineEnd.x - LineStart.x);
    Intersection.y = LineStart.y + U * (LineEnd.y - LineStart.y);

    // --------------------------------------------------------------------
    // find the distance from the point to the line intersection
    // --------------------------------------------------------------------

    float Distance = Vector2Distance(Point, Intersection);

    return Distance;
}
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(Vector2& Point, Vector2& LineStart,
                                   Vector2& LineEnd) {
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
closest_line(const std::vector<std::pair<float, Vector2>>& distances) {

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
[[nodiscard]] Vector2 closest_point(Vector2 target, const std::vector<Vector2>& points) {

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

void debug_print_animation_info(const AnimationInfo& animation_info) {
    PRINT(animation_info.current_region_rect.x);
    PRINT(animation_info.current_region_rect.y);
    // PRINT(animation_info.current_region_rect.width);
    // PRINT(animation_info.current_region_rect.height);
    PRINT(animation_info.current_sheet_name);
    PRINT(animation_info.frame_progress);
    std::cerr << '\n';
}
