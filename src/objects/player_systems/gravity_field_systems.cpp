

#include "fumo_engine/global_state.hpp"
#include "fumo_engine/scheduling_systems.hpp"
#include "main_functions.hpp"
#include "player_physics.hpp"

extern std::unique_ptr<GlobalState> global;

void LevelGravityHandler::find_gravity_field() {
    // RULES:
    // - give priority to orbit change
    // - don't change orbits if the player is on the ground
    // - only allow picking candidates again if the player touches the ground

    std::vector<EntityId> candidate_planets;
    candidate_planets.reserve(sys_entities.size());

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);

    if (player_body.touching_ground) {
        return;
    }

    EntityQuery query_parallel{
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};

    for (const auto& planet_id : sys_entities) {
        if (global->ECS->filter(planet_id, query_parallel)) {
            // methods for parallel gravity fields
            const auto& parallel_field =
                global->ECS->get_component<ParallelGravityField>(planet_id);
            if (parallel_field.is_inside_field(player_body, player_shape)) {
                candidate_planets.push_back(planet_id);
            }
        } else {
            // methods for circular gravity fields
            const auto& circular_field =
                global->ECS->get_component<CircularGravityField>(planet_id);
            if (circular_field.is_inside_field(player_body, player_shape)) {
                candidate_planets.push_back(planet_id);
            }
        }
    }

    // -------------------------------------------------------------------------------
    // pick the final field
    // -------------------------------------------------------------------------------
    // NOTE:
    // - we can (hopefully) assume the player is at most inside 2 fields!!!
    // dont add more than 2 fields, if you do, change the code to support that

    for (const auto& planet_id : candidate_planets) {
        if (planet_id != player_shape.player_owning_field) {
            player_shape.player_owning_field = planet_id;
            player_body.jumping = false;

            const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
            scheduler_system->sleep_unregistered_system<GravityHandler>();
            scheduler_system
                ->awake_unregistered_system_priority<GravityBufferHandler, 8>();
        }
    }
}

bool ParallelGravityField::is_inside_field(const Body& player_body,
                                           const PlayerShape& player_shape) const {

    // FIXME: change to this checking both of the player circles for collisions
    Rectangle rect;
    CircleShape circle;
    Vector2 circle_center;

    Vector2 TopLeft = {rect.x, rect.y};
    Vector2 TopRight = {rect.x + rect.width, rect.y};
    Vector2 BottomLeft = {rect.x, rect.y + rect.height};
    Vector2 BottomRight = {rect.x + rect.width, rect.y + rect.height};

    std::pair<float, Vector2> distance_to_left, distance_to_bottom, distance_to_right,
        distance_to_top;

    distance_to_left =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
    distance_to_top =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
    distance_to_right =
        PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
    distance_to_bottom =
        PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);

    std::vector<std::pair<float, Vector2>> distances = {
        distance_to_left, distance_to_top, distance_to_right, distance_to_bottom};

    std::pair<float, Vector2> closest_pair = closest_line(distances);

    // -------------------------------------------------------------------------------
    float intersection_distance = closest_pair.first;
    Vector2 closest_intersection = closest_pair.second;

    if (intersection_distance == 0) {
        // check the corner of the rectangle
        std::vector<Vector2> points{TopLeft, TopRight, BottomLeft, BottomRight};
        closest_intersection = closest_point(circle_center, points);
        intersection_distance = Vector2Distance(circle_center, closest_intersection);

        // if true, then there was a collision with the corner
        return (intersection_distance < circle.radius);
    }
    // -------------------------------------------------------------------------------
    // no collision happened, not inside the field
    return false;

}

// FIXME: finish writing the rectangle code
// and change to this checking both of the player circles for collisions
void circle_to_rectangle_collision_solving() {

    // NOTE: assume rectangles are NOT rotated
    // (add rotation support later if i want to add slopes and stuff)

    // -------------------------------------------------------------------------------
    // Explanation:
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

    Rectangle rect;
    CircleShape circle;
    Vector2 circle_center;

    Vector2 TopLeft = {rect.x, rect.y};
    Vector2 TopRight = {rect.x + rect.width, rect.y};
    Vector2 BottomLeft = {rect.x, rect.y + rect.height};
    Vector2 BottomRight = {rect.x + rect.width, rect.y + rect.height};

    std::pair<float, Vector2> distance_to_left, distance_to_bottom, distance_to_right,
        distance_to_top;

    distance_to_left =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, BottomLeft);
    distance_to_top =
        PointToLineDistanceAndIntersection(circle_center, TopLeft, TopRight);
    distance_to_right =
        PointToLineDistanceAndIntersection(circle_center, TopRight, BottomRight);
    distance_to_bottom =
        PointToLineDistanceAndIntersection(circle_center, BottomLeft, BottomRight);

    std::vector<std::pair<float, Vector2>> distances = {
        distance_to_left, distance_to_top, distance_to_right, distance_to_bottom};

    std::pair<float, Vector2> closest_pair = closest_line(distances);

    // -------------------------------------------------------------------------------
    // solve overlapping
    // -------------------------------------------------------------------------------
    float intersection_distance = closest_pair.first;
    Vector2 closest_intersection = closest_pair.second;

    // -------------------------------------------------------------------------------
    // if no side a direct collision, then
    // find the closest corner and move away from it
    if (intersection_distance == 0) {
        std::vector<Vector2> points{TopLeft, TopRight, BottomLeft, BottomRight};
        closest_intersection = closest_point(circle_center, points);
        intersection_distance = Vector2Distance(circle_center, closest_intersection);
    }
    // -------------------------------------------------------------------------------

    float overlap = circle.radius - intersection_distance;
    float correction = 1.0f; // NOTE: this value is here to stabilize the collision

    Vector2 push_direction = circle_center - closest_intersection;
    Vector2 push = Vector2Normalize(push_direction);

    push = Vector2Scale(push, overlap * correction);
    circle_center += push;
}
