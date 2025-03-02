#include "main_functions.hpp"
Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
    Vector2 result = {v.x / scale, v.y / scale};
    return result;
}
[[nodiscard]] float PointToLineDistance(Vector2& Point, Vector2& LineStart, Vector2& LineEnd) {
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
// Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
//     Vector2 result = {v.x / scale, v.y / scale};
//     return result;
// }
// inline float Magnitude(Vector2& Point1, Vector2& Point2) {
//     Vector2 Vector;
//
//     Vector.x = Point2.x - Point1.x;
//     Vector.y = Point2.y - Point1.y;
//
//     return sqrtf(Vector.x * Vector.x + Vector.y * Vector.y);
// }
//
// float PointToLineDistance(Vector2& Point, Vector2& LineStart, Vector2& LineEnd) {
//     float LineMag;
//     float U;
//     Vector2 Intersection;
//
//     LineMag = Magnitude(LineEnd, LineStart);
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

//     return Magnitude(Point, Intersection);
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
