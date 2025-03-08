
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/core/scheduling_systems.hpp"
#include "fumo_engine/collisions_and_physics/gravity_field_systems.hpp"
#include "objects/systems.hpp"

extern std::unique_ptr<GlobalState> global;

void GravityBufferHandler::wait_for_touching_ground() {

    auto& body = global->ECS->get_component<Body>(global->player_id);

    if (body.touching_ground) {
        // garbage fix for the shitty player jump
        body.iterations = 0;
        body.going_down = false;
        body.going_down = false;
        const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
        // scheduler_system->awake_unregistered_system<GravityHandler>();
        scheduler_system->awake_unregistered_system<LevelGravityHandler>();
    }
}
void GravityUpdater::gravity_update() {

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    update_gravity(player_body);
    update_position(player_body);
}
void GravityUpdater::update_gravity(Body& body) {
    // NOTE: points towards the planet's planet_body

    if (player_owning_planet == NO_ENTITY_FOUND || player_owning_planet == 0) {
        return;
    }
    EntityId planet_id = player_owning_planet;

    const auto& planet_body = global->ECS->get_component<Body>(planet_id);
    const auto& gravity_field = global->ECS->get_component<GravityField>(planet_id);
    const auto& circle_shape = global->ECS->get_component<Circle>(planet_id);

    auto& entity_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<Circle>(global->player_id);

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
    entity_body.x_direction = x_direction;
    entity_body.rotation = std::atan2(x_direction.y, x_direction.x) * RAD2DEG;

    // mario galaxy-like changing of the left-right movement to match
    // the player expectation of what right and left should be
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
        if (160 < abs(entity_body.rotation) && abs(entity_body.rotation) < 180) {
            entity_body.inverse_direction = true;
        } else if (0 < abs(entity_body.rotation) && abs(entity_body.rotation) < 20) {
            entity_body.inverse_direction = false;
        }
    }

    //-------------------------------------------------------------------
    float distance = Vector2Distance(planet_body.position, entity_body.position);
    float radius_sum = player_shape.radius + circle_shape.radius;
    float correction = 2.0f;
    bool touching_ground = distance < radius_sum + correction;
    entity_body.touching_ground = touching_ground;
    //-------------------------------------------------------------------

    const auto& jump_physics = global->ECS->get_system<JumpPhysicsHandler>();
    if (jump_physics->hard_coded_jump()) {
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
}

void GravityUpdater::update_position(Body& player_body) {
    player_body.position += player_body.velocity * global->frametime;
}
// void smoothen_jump(std::tuple<Body, GravityField, CircleShape, EntityId>&
// final_planet) {
//
//     auto& player_body = global->ECS->get_component<Body>(global->player_id);
//     auto& player_shape = global->ECS->get_component<CircleShape>(global->player_id);
//     const auto& planet_body = std::get<0>(final_planet);
//     const auto& gravity_field = std::get<1>(final_planet);
//     const auto& circle_shape = std::get<2>(final_planet);
//     player_body.velocity -= player_body.gravity_direction * 10.0f;
//
//     float distance = Vector2Distance(planet_body.position, player_body.position);
//     float radius_sum = player_shape.radius + circle_shape.radius;
//     float gravity_radius_sum = radius_sum + gravity_field.gravity_radius;
//     //-------------------------------------------------------------------
//     float correction = 2.0f;
//     // bool touching_ground = distance < radius_sum + correction;
//     bool inside_field = distance < gravity_radius_sum;
//     //-------------------------------------------------------------------
//     if (inside_field) {
//         player_body.jumping = false;
//     }
// }
