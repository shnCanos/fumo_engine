#include "player_physics.hpp"
#include "fumo_engine/global_state.hpp"
#include "fumo_engine/sprite_manager/sprite_and_animation_systems.hpp"
#include "objects/components.hpp"
#include "raylib.h"
#include "raymath.h"

extern std::unique_ptr<GlobalState> global;

void PlayerPhysicsRunner::run_physics() {

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<CircleShape>(global->player_id);
    update_gravity(player_body, player_shape);
    update_position(player_body);
}

void PlayerPhysicsRunner::update_gravity(Body& player_body, CircleShape& player_shape) {
    auto circle_physics_ptr = global->ECS->get_system<CirclePhysicsHandler>();
    // auto rect_physics_ptr = global->ECS->get_system<RectanglePhysicsHandler>();

    circle_physics_ptr->find_gravity_field(player_body, player_shape);
}

void PlayerPhysicsRunner::update_position(Body& player_body) {
    player_body.position += player_body.velocity * global->frametime;
}

void CirclePhysicsHandler::find_gravity_field(Body& entity_body,
                                              CircleShape& entity_shape) {
    // NOTE: we should only let one planet affect the player at each time

    for (auto circle_id : sys_entities) {
        // FIXME: check that "Only" filter will remove the player id from the array

        // FIXME: add a buffer that doesnt allow swapping orbits more
        // than once a second
        const auto& circle_body = global->ECS->get_component<Body>(circle_id);
        const auto& circle_shape = global->ECS->get_component<CircleShape>(circle_id);
        const auto& circle_grav_field =
            global->ECS->get_component<GravityField>(circle_id);

        float distance = Vector2Distance(circle_body.position, entity_body.position);
        float radius_sum = entity_shape.radius + circle_shape.radius;

        float gravity_radius_sum = radius_sum + circle_grav_field.gravity_radius;

        //-------------------------------------------------------------------
        // correction is the leeway i give to stop adding gravity to the body
        // (important to properly identify if we are touching the group)
        float correction = 2.0f;
        bool touching_ground = distance < radius_sum + correction;
        bool inside_field = distance < gravity_radius_sum;
        //-------------------------------------------------------------------

        entity_body.touching_ground = touching_ground;
        // PRINT(entity_body.touching_ground);

        if (inside_field) {
            update_gravity(circle_grav_field, circle_body, entity_body);
        }
    }
}

bool dont_look_bad_hard_coded_physics(Body& entity_body) {

    if (entity_body.jumping) {
        // going up smoothing
        if (entity_body.going_up) {
            entity_body.iterations++;
            // PRINT(entity_body.iterations);
            entity_body.scale_velocity(-20.0f /
                                       (entity_body.iterations * global->frametime));
            if (entity_body.iterations == 10) {
                entity_body.going_up = false;
                entity_body.going_down = true;
                entity_body.iterations = 0;
            }
        }
        // going down smoothing
        if (entity_body.going_down) {
            // 25000 at least downwards
            entity_body.iterations++;
            // PRINT(entity_body.iterations);
            entity_body.scale_velocity(5000.0f * entity_body.iterations *
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

void CirclePhysicsHandler::update_gravity(const GravityField& circle_grav_field,
                                          const Body& circle_body, Body& entity_body) {

    // points towards the planet's circle_body
    Vector2 gravity_direction =
        Vector2Normalize(circle_body.position - entity_body.position);
    entity_body.gravity_direction = gravity_direction;

    // remove the y component from the velocity
    // by setting the velocity to its magnitude in the x direction
    // some funky rotation matrix vector math going on here, check it on paper
    // if you are confused
    Vector2 x_direction = {entity_body.gravity_direction.y,
                           -entity_body.gravity_direction.x};
    entity_body.rotation =
        std::atan2(x_direction.y, x_direction.x) * RAD2DEG;

    if (dont_look_bad_hard_coded_physics(entity_body)) {
        return;
    }

    if (!entity_body.touching_ground) {
        Vector2 acceleration =
            gravity_direction * circle_grav_field.gravity_strength * 1000.0f;
        entity_body.velocity += acceleration * global->frametime;
        // PRINT(entity_body.get_dot_y_velocity());
    } else {
        entity_body.velocity =
            x_direction * Vector2DotProduct(entity_body.velocity, x_direction);
        const auto& animation_player = global->ECS->get_system<AnimationPlayer>();
        auto& animation_info = global->ECS->get_component<AnimationInfo>(global->player_id);
    }

    // PRINT(entity_body.velocity.x);
    // PRINT(entity_body.velocity.y);
}

// void RectanglePhysicsHandler::update_gravity(Body& entity_body) {
//     for (auto rectangle_id : sys_entities) {
//         auto rectangle_body = global->ECS->get_component<Body>(rectangle_id);
//         auto rect_shape = global->ECS->get_component<RectangleShape>(rectangle_id);
//         // TODO: change the Y axis of the body (player) to match
//         // the side of the rectangle we are on vertically
//     }
// }
// for (auto circle_id : sys_entities) {
//     auto circle_body = global->ECS->get_component<Body>(circle_id);
//     auto circle_shape = global->ECS->get_component<CircleShape>(circle_id);
//     // TODO: change the Y axis of the body (player) to match
//     // the center of the circle planet
//
//     Vector2 force_direction =
//         Vector2Normalize(circle_body.position - entity_body.position);
//     // F = (G x m1 x m2) / r²; F = m x a
//     // rewritten as:
//     //       a = (G x m2) / r²
//     float sqr_distance =
//         Vector2DistanceSqr(circle_body.position, entity_body.position);
//     Vector2 acceleration =
//         force_direction * GRAVITATIONAL_CONSTANT * circle_body.mass / sqr_distance;
//     // multiplied by direction to turn magnitude into vectorial change
//     entity_body.velocity += acceleration * global->frametime;
//
