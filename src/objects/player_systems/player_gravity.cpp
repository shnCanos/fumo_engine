#include "fumo_engine/global_state.hpp"
#include "fumo_engine/scheduling_systems.hpp"
#include "main_functions.hpp"
#include "objects/components.hpp"
#include "objects/player_systems/player_physics.hpp"
#include "raylib.h"
#include <vector>

extern std::unique_ptr<GlobalState> global;
bool dont_look_bad_hard_coded_physics(Body& entity_body);

void GravityUpdater::gravity_update() {

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    update_gravity(player_body);
    update_position(player_body);
}

void GravityHandler::find_candidate_gravity_field() {
    // NOTE: we should only let one planet affect the player at each time

    std::vector<std::tuple<Body, GravityField, CircleShape, EntityId>> candidate_planets;
    candidate_planets.reserve(sys_entities.size());

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<CircleShape>(global->player_id);

    for (const auto& planet_id : sys_entities) {
        const auto& planet_body = global->ECS->get_component<Body>(planet_id);
        const auto& circle_shape = global->ECS->get_component<CircleShape>(planet_id);
        auto& gravity_field = global->ECS->get_component<GravityField>(planet_id);

        float distance = Vector2Distance(planet_body.position, player_body.position);
        float radius_sum = player_shape.radius + circle_shape.radius;

        float gravity_radius_sum = radius_sum + gravity_field.gravity_radius;

        //-------------------------------------------------------------------
        // correction is the leeway i give to stop adding gravity to the body
        // (important to properly identify if we are touching the group)
        float correction = 2.0f;
        bool touching_ground = distance < radius_sum + correction;
        bool inside_field = distance < gravity_radius_sum;
        //-------------------------------------------------------------------

        player_body.touching_ground = touching_ground;

        if (inside_field) {
            candidate_planets.emplace_back(
                std::tuple<Body, GravityField, CircleShape, EntityId>(
                    planet_body, gravity_field, circle_shape, planet_id));
        }
    }

    DrawLineV(player_body.position, player_body.position + player_body.velocity, PURPLE);

    if (candidate_planets.size() == 1) {

        const auto& gravity_updater = global->ECS->get_system<GravityUpdater>();
        gravity_updater->player_owning_planet = std::get<3>(candidate_planets[0]);
        return;
    }
    find_player_owning_gravity_field(candidate_planets, player_body);
}

void GravityHandler::find_player_owning_gravity_field(
    std::vector<std::tuple<Body, GravityField, CircleShape, EntityId>>&
        candidate_planets,
    Body& player_body) {

    std::vector<std::tuple<Body, GravityField, CircleShape, EntityId>>
        final_candidate_planets;
    final_candidate_planets.reserve(candidate_planets.size());

    for (auto& planet_tuple : candidate_planets) {

        auto& planet_body = std::get<0>(planet_tuple);
        auto& gravity_field = std::get<1>(planet_tuple);
        auto& planet_shape = std::get<2>(planet_tuple);

        // --------------------------------------------------------------------
        // how it works:
        // we cast a ray (line) along the player's velocity
        // and we check if it collided with any planets
        // the closest planet the ray collied with is
        // our new gravity direction owner
        // --------------------------------------------------------------------

        // WARNING: this variable decides how far we check for planets to swap
        // our gravity. it is important to pick a good value
        double gravity_reach = 500.0f;

        // TODO: draw this line with raylib to check how far we might want to draw it in
        // the future

        Vector2 normalized_velocity = Vector2Normalize(player_body.velocity);

        Vector2 line_end = player_body.position + normalized_velocity * gravity_reach;

        DrawLineV(player_body.position, line_end, YELLOW);

        float distance =
            PointToLineDistance(planet_body.position, player_body.position, line_end);
        float correction = 2.0f;

        if (distance < gravity_field.gravity_radius + planet_shape.radius + correction) {
            // this means that we found a collision with our cast ray
            // so we move onto the last phase of checking for gravity
            final_candidate_planets.push_back(planet_tuple);
        }
    }
    find_final_candidate_gravity_field(final_candidate_planets, player_body);
}

void GravityHandler::find_final_candidate_gravity_field(
    std::vector<std::tuple<Body, GravityField, CircleShape, EntityId>>&
        final_candidate_planets,
    Body& player_body) {

    float min_distance = 6969.0f;

    std::tuple<Body, GravityField, CircleShape, EntityId> final_planet;

    for (const auto& planet_tuple : final_candidate_planets) {
        const auto& planet_body = std::get<0>(planet_tuple);
        const auto& gravity_field = std::get<1>(planet_tuple);
        const auto& circle_shape = std::get<2>(planet_tuple);

        float distance = Vector2Distance(player_body.position, planet_body.position);
        if (distance < min_distance) {
            min_distance = distance;
            final_planet = planet_tuple;
        }
    }
    const auto& gravity_updater = global->ECS->get_system<GravityUpdater>();
    gravity_updater->player_owning_planet = std::get<3>(final_planet);

    if (gravity_updater->player_owning_planet != std::get<3>(final_planet)) {
        // FIXME: test and debug these functions and check if it really
        // sleeps this system for 2 seconds
        const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
        scheduler_system->sleep_system_for<GravityHandler>(2.0f);
    }
}

void GravityUpdater::update_gravity(Body& body) {
    // NOTE: points towards the planet's planet_body

    if (player_owning_planet == NO_ENTITY_FOUND || player_owning_planet == 0) {
        return;
    }
    EntityId planet_id = player_owning_planet;

    const auto& planet_body = global->ECS->get_component<Body>(planet_id);
    const auto& gravity_field = global->ECS->get_component<GravityField>(planet_id);

    auto& entity_body = global->ECS->get_component<Body>(global->player_id);

    Vector2 gravity_direction =
        Vector2Normalize(planet_body.position - entity_body.position);
    entity_body.gravity_direction = gravity_direction;

    // --------------------------------------------------------------------
    // remove the y component from the velocity
    // by setting the velocity to its magnitude in the x direction
    // some funky rotation matrix vector math going on here, check it on paper
    // if you are confused
    // --------------------------------------------------------------------

    Vector2 x_direction = {entity_body.gravity_direction.y,
                           -entity_body.gravity_direction.x};
    entity_body.rotation = std::atan2(x_direction.y, x_direction.x) * RAD2DEG;

    if (dont_look_bad_hard_coded_physics(entity_body)) {
        return;
    }

    if (!entity_body.touching_ground) {
        Vector2 acceleration =
            gravity_direction * gravity_field.gravity_strength * 1000.0f;
        entity_body.velocity += acceleration * global->frametime;
    } else {
        entity_body.velocity =
            x_direction * Vector2DotProduct(entity_body.velocity, x_direction);
    }

    PRINT(entity_body.position.x)
    PRINT(entity_body.position.y)
    PRINT(entity_body.touching_ground)
    // PRINT(entity_body.position.x)
    // PRINT(entity_body.position.x)
    // PRINT(entity_body.position.x)
}

void GravityUpdater::update_position(Body& player_body) {
    player_body.position += player_body.velocity * global->frametime;
}

bool dont_look_bad_hard_coded_physics(Body& entity_body) {
    // NOTE: this code is for testing and will be removed later

    if (entity_body.jumping) {
        // going up smoothing
        if (entity_body.going_up) {
            entity_body.iterations++;
            entity_body.scale_velocity(-50.0f /
                                       (entity_body.iterations * global->frametime));
            if (entity_body.iterations < 10) {
                return true;
            }

            entity_body.scale_velocity(-5.0f /
                                       (entity_body.iterations * global->frametime));

            if (entity_body.iterations == 17) {
                entity_body.going_up = false;
                entity_body.going_down = true;
                entity_body.iterations = 0;
            }
        }
        // going down smoothing
        if (entity_body.going_down) {
            // 25000 at least downwards
            entity_body.iterations++;
            entity_body.scale_velocity(3000.0f * entity_body.iterations *
                                       global->frametime);
            if (entity_body.iterations == 10) {
                entity_body.jumping = false;
                entity_body.going_down = false;
                entity_body.iterations = 0;
            }
        }
        return true;
    }
    return false;
}
