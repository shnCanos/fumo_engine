
#include "fumo_engine/collisions_and_physics/gravity_field_systems.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/systems.hpp"

extern std::unique_ptr<GlobalState> global;

void GravityBufferHandler::wait_for_touching_ground() {

    auto& player_flag = global->ECS->get_component<PlayerFlag>(global->player_id);
    auto& body = global->ECS->get_component<Body>(global->player_id);

    if (body.on_ground) {
        // garbage fix for the shitty player jump
        body.iterations = 0;
        body.going_down = false;
        body.going_down = false;

        player_flag.can_swap_orbits = true;
        PRINT("ALLOWED TO SWAP ORBITS");

        // TODO: (check if this is actually more performant)
        // turn itself off to avoid busy wait
        // const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
        // scheduler_system->sleep_unregistered_system<GravityBufferHandler>();
        // scheduler_system->awake_system<GravityHandler>();
    }
}

void GravityUpdater::gravity_update() {

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    update_gravity(player_body);
}

void GravityUpdater::update_gravity(Body& body) {
    // NOTE: points towards the planet's planet_body

    if (player_owning_planet == NO_ENTITY_FOUND || player_owning_planet == 0) {
        return;
    }
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);

    // --------------------------------------------------------------------
    // mario galaxy-like changing of the left-right movement to match
    // the player expectation of what right and left should be
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
        if (160 < abs(player_body.rotation) && abs(player_body.rotation) < 180) {
            player_body.inverse_direction = true;
        } else if (0 < abs(player_body.rotation) && abs(player_body.rotation) < 20) {
            player_body.inverse_direction = false;
        }
    }
    // --------------------------------------------------------------------
    Vector2 gravity_direction{};
    float gravity_strength{};

    EntityId planet_id = player_owning_planet;

    const auto& body_planet = global->ECS->get_component<Body>(planet_id);

    EntityQuery query_parallel{
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};

    if (global->ECS->filter(planet_id, query_parallel)) {
        // methods for parallel gravity fields
        auto& parallel_field =
            global->ECS->get_component<ParallelGravityField>(planet_id);
        parallel_field.update_gravity(player_body);

        gravity_direction = parallel_field.gravity_direction;
        gravity_strength = parallel_field.gravity_strength;
    } else {
        // methods for circular gravity fields
        auto& circular_field =
            global->ECS->get_component<CircularGravityField>(planet_id);
        const auto& circle_shape = global->ECS->get_component<Circle>(planet_id);
        circular_field.update_gravity(player_body, body_planet);

        gravity_direction = circular_field.gravity_direction;
        gravity_strength = circular_field.gravity_strength;
    }
    // --------------------------------------------------------------------

    player_body.gravity_direction = gravity_direction;

    // --------------------------------------------------------------------
    const auto& jump_physics = global->ECS->get_system<JumpPhysicsHandler>();

    if (jump_physics->hard_coded_jump()) {
        return;
    }
    // if (player_body.on_ground) {
    //     // dont update while player is on the ground
    //     return;
    // }
    // --------------------------------------------------------------------
    Vector2 acceleration = gravity_direction * gravity_strength * 1000.0f;
    player_body.velocity += acceleration * global->frametime;
}

void ParallelGravityField::update_gravity(Body& player_body) {

    // do nothing, parallel fields stay the same unless they rotate
    // TODO: implement updating based on rotation in this function
    //
}

void CircularGravityField::update_gravity(Body& player_body, const Body& body_planet) {
    gravity_direction = Vector2Normalize(body_planet.position - player_body.position);
}
