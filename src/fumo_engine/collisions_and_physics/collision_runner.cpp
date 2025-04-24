#include "fumo_engine/collisions_and_physics/collision_runner.hpp"

#include "fumo_engine/collisions_and_physics/collision_functions.hpp"
#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

void CollisionRunner::check_collisions() {
    // TODO: change the collisions to use raycasting (so they work very consistently)
    // (do this later once its finished since it is good enough for now)
    bool collision_happened = false;
    constexpr float substep_count = 10;

    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_capsule =
        global->ECS->get_component<Capsule>(global->player_id);
    auto& player_state =
        global->ECS->get_component<EntityState>(global->player_id);

    const EntityQuery fumo_rect_query {
        .component_mask = global->ECS->make_component_mask<FumoRect>(),
        .component_filter = Filter::All};

    const EntityQuery circle_query {
        .component_mask = global->ECS->make_component_mask<FumoRect>(),
        .component_filter = Filter::All};

    for (const auto& obstacle_id : sys_entities) {
        const auto& obstacle_body =
            global->ECS->get_component<Body>(obstacle_id);

        if (global->ECS->filter(obstacle_id, fumo_rect_query)) {
            const auto& fumo_rect =
                global->ECS->get_component<FumoRect>(obstacle_id);

            if (Collisions::continuous_rect_collision_solving(player_capsule,
                                                              player_body,
                                                              fumo_rect,
                                                              obstacle_body,
                                                              substep_count)) {
                collision_happened = true;
            }
            continue;
        }
        const auto& circle_shape =
            global->ECS->get_component<Circle>(obstacle_id);
        if (Collisions::capsule_to_circle_collision_solving(player_capsule,
                                                            player_body,
                                                            circle_shape,
                                                            obstacle_body,
                                                            substep_count)) {
            collision_happened = true;
        }
    }

    player_state.colliding = collision_happened;
    if (collision_happened) {
        global->event_handler->add_event({EVENT_::ENTITY_COLLIDED, player_id});
        return;
    }
}

#define CHECK_COLLISION(Shape, query, function_name) \
    do { \
        if (global->ECS->filter(obstacle_id, query)) { \
            const auto& shape = \
                global->ECS->get_component<Shape>(obstacle_id); \
            Collision collision = \
                Collisions::function_name(line, shape, obstacle_body); \
            if (collision.collided \
                && (collision.distance < closest_collision.distance \
                    || closest_collision.distance == 0.0f)) { \
                closest_collision = collision; \
            } \
            continue; \
        } \
    } while (0)

[[nodiscard]] const Collision
CollisionRunner::check_raycast_line(const Line& line) {

    Collision closest_collision;

    const EntityQuery fumo_rect_query {
        .component_mask = global->ECS->make_component_mask<FumoRect>(),
        .component_filter = Filter::All};

    const EntityQuery circle_query {
        .component_mask = global->ECS->make_component_mask<Circle>(),
        .component_filter = Filter::All};

    for (const auto& obstacle_id : sys_entities) {
        const auto& obstacle_body =
            global->ECS->get_component<Body>(obstacle_id);

        CHECK_COLLISION(FumoRect, fumo_rect_query, LineToRectCollision);
        CHECK_COLLISION(Circle, circle_query, LineToCircleCollision);
    }
    return closest_collision;
}
