#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
//
// namespace Collisions {
//
//
//
// [[nodiscard]] Collision RayEndToCircleCollision(const Line& line,
//                                                 const Circle& circle_shape,
//                                                 const Body& circle_body) {
//     // NOTE: only checking the end of the line because
//     // the beginning of the ray should never be inside the circle
//
//     Collision collision {};
//
//     float distance = FumoVec2Distance(circle_body.position, line.end);
//     if (distance < circle_shape.radius) {
//
//         collision.distance = distance;
//         collision.overlap = circle_shape.radius - collision.distance;
//         collision.intersection_point = line.end;
//         collision.normal_or_push = FumoVec2Normalize(
//             collision.intersection_point - circle_body.position);
//         // in this case are getting the normal to the surface always
//         collision.normal = collision.normal_or_push;
//         collision.collided = true;
//
//         PRINT_NO_NAME("LINE END")
//     }
//
//     return collision;
// }
//
// [[nodiscard]] const Collision LineToCircleCollision(const Line& line,
//                                                     const Circle& circle_shape,
//                                                     const Body& circle_body) {
//
//     Collision closest_collision =
//         RayEndToCircleCollision(line, circle_shape, circle_body);
//
//     Collision collision = PointToLineCollision(circle_body.position, line);
//
//     if (collision.collided && collision.distance < circle_shape.radius
//         && (collision.distance < closest_collision.distance
//             || closest_collision.distance == 0.0f)) {
//
//         collision.overlap = circle_shape.radius - collision.distance;
//         collision.normal_or_push = FumoVec2Normalize(
//             collision.intersection_point - circle_body.position);
//         collision.normal = collision.normal_or_push;
//         closest_collision = collision;
//     }
//     return closest_collision;
// }
//
// } // namespace Collisions
