#include <raylib.h>

#include "fumo_engine/collisions_and_physics/gravity_field_systems.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_raylib.hpp"

extern std::unique_ptr<GlobalState> global;

void GravityUpdater::gravity_update() {
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    update_gravity(global->player_id, player_body);
}

void GravityUpdater::update_gravity(EntityId entity_id, Body& player_body) {
    // NOTE: points towards the planet's planet_body
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);

    if (player_state.player_owning_field == NO_ENTITY_FOUND
        || player_state.player_owning_field == 0) {
        return;
    }

    float gravity_strength {};

    EntityId planet_id = player_state.player_owning_field;
    EntityQuery query_parallel {
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::All};

    const auto& body_planet = global->ECS->get_component<Body>(planet_id);
    FumoVec2 gravity_direction {};

    // --------------------------------------------------------------------
    // mario galaxy-like changing of the left-right movement to match
    // the player expectation of what right and left should be
    EntityQuery skip_parallel {
        .component_mask = global->ECS->make_component_mask<ParallelGravityField>(),
        .component_filter = Filter::None};

    // --------------------------------------------------------------------

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

    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // FIXME:: replace gravity updating so that we have smoothing or
    // at least dont add up a fixed value

    float factor = 6;

    if (player_state.dashing) {
        factor = 0;

    player_body.gravity_direction = {player_body.x_direction.y, -player_body.x_direction.x};
    } else {
        player_body.gravity_direction = gravity_direction;

        if (player_state.jumping) {
            float jump_progress =
                FumoVec2DotProduct(player_body.get_y_velocity(), gravity_direction);

            if (jump_progress > 0) {
                factor += 4;
            }

            if (IsKeyReleased(KEY_SPACE)) {
                player_body.velocity = player_body.get_x_velocity()
                    + player_body.get_y_velocity() * 0.5f;
            }

            if (!IsKeyDown(KEY_SPACE)) {
                factor += 2;
            }
        }
    }

    FumoVec2 acceleration = gravity_direction * gravity_strength * factor;
    player_body.velocity += acceleration * global->frametime;
}

void ParallelGravityField::update_gravity(Body& player_body) {
    // do nothing, parallel fields stay the same unless they rotate
    // TODO: implement updating based on rotation in this function
    //
}

void CircularGravityField::update_gravity(Body& player_body,
                                          const Body& body_planet) {
    gravity_direction =
        FumoVec2Normalize(body_planet.position - player_body.position);
}
