#include "fumo_engine/collisions_and_physics/gravity_field_systems.hpp"
#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/core/scheduling_systems.hpp"

extern std::unique_ptr<GlobalState> global;

void GravityHandler::find_gravity_field() {
    // RULES:
    // - give priority to orbit change
    // - don't change orbits if the player is on the ground
    // - only allow picking candidates again if the player touches the ground

    std::vector<EntityId> candidate_planets;
    candidate_planets.reserve(sys_entities.size());

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);

    if (player_body.touching_ground) {
        // dont check orbits when we are on the ground
        return;
    }

    EntityQuery query_parallel{
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};

    for (const auto& planet_id : sys_entities) {

        const auto& body_planet = global->ECS->get_component<Body>(planet_id);

        if (global->ECS->filter(planet_id, query_parallel)) {
            // methods for parallel gravity fields
            const auto& parallel_field =
                global->ECS->get_component<ParallelGravityField>(planet_id);
            if (parallel_field.is_inside_field(player_body, player_shape, body_planet)) {
                candidate_planets.push_back(planet_id);
            }
        } else {
            // methods for circular gravity fields
            const auto& circular_field =
                global->ECS->get_component<CircularGravityField>(planet_id);
            if (circular_field.is_inside_field(player_body, player_shape, body_planet)) {
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
            // -------------------------------------------------------------------------------
            // check if you want to cancel the jump this way later on
            // solution:
            // - the jump only moves up, the gravity pulls it down
            // (im not sure if this is feasible to get a  nice jump curve)
            // - i dont think it is feasible to find a good gravity value
            // that makes the jump as smooth as we want it to be,
            // but we can try
            player_body.jumping = false;
            // -------------------------------------------------------------------------------

            // FIXME: add this back when the code is working (gravity handler has been
            // redone)
            //
            const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
            scheduler_system->sleep_unregistered_system<GravityHandler>();
            scheduler_system
                ->awake_unregistered_system_priority<GravityBufferHandler, 8>();
        }
    }
}

bool ParallelGravityField::is_inside_field(const Body& player_body,
                                           const PlayerShape& player_shape,
                                           const Body& body_parallel) const {

    Collision collision =
        PlayerToRectCollision(player_shape, player_body, field_rectangle, body_parallel);

    // if overlap == 0, then there was no collision
    return (collision.overlap == 0);
}

bool CircularGravityField::is_inside_field(const Body& player_body,
                                           const PlayerShape& player_shape,
                                           const Body& circular_body) const {

    // -------------------------------------------------------------------------------
    // check for collisions with capsule
    // -------------------------------------------------------------------------------
    float radius_sum = player_shape.radius + gravity_radius;

    // -------------------------------------------------------------------------------
    // top circle collision check
    float top_distance =
        Vector2Distance(circular_body.position, player_shape.top_circle_center);
    if (top_distance < radius_sum) {
        // collided with top circle
        return true;
    }
    // -------------------------------------------------------------------------------
    // bottom circle collision check
    float bottom_distance =
        Vector2Distance(circular_body.position, player_shape.bottom_circle_center);
    if (bottom_distance < radius_sum) {
        // collided with top circle
        return true;
    }

    // -------------------------------------------------------------------------------
    // TODO: remove the side checks later
    //
    // capsule sides collision check
    const auto left_line_distance_pair = PointToLineDistanceAndIntersection(
        circular_body.position, player_shape.left_line);

    if (left_line_distance_pair.first != 0 &&
        left_line_distance_pair.first < gravity_radius) {
        // collided with left_line
        return true;
    }

    const auto right_line_distance_pair = PointToLineDistanceAndIntersection(
        circular_body.position, player_shape.right_line);

    if (right_line_distance_pair.first != 0 &&
        right_line_distance_pair.first < gravity_radius) {
        // collided with right_line
        return true;
    }
    // -------------------------------------------------------------------------------
    return false;
}
