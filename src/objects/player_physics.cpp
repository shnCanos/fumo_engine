#include "player_physics.hpp"
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"

extern std::unique_ptr<GlobalState> global;
void PlayerPhysicsRunner::update_gravity() {
    auto circle_physics_ptr = global->ECS.get_system<CirclePhysicsHandler>();
    auto rect_physics_ptr = global->ECS.get_system<RectanglePhysicsHandler>();

    circle_physics_ptr->update_gravity(global->player_id);
}

void CirclePhysicsHandler::update_gravity(EntityId entity_id) {
    for (auto circle_id : sys_entities) {
        auto circle_body = global->ECS.get_component<Body>(circle_id);
        auto entity_body = global->ECS.get_component<Body>(entity_id);
        auto circle_shape = global->ECS.get_component<CircleShape>(circle_id);
        // TODO: change the Y axis of the player to match
        // the center of the circle planet
    }
}

void RectanglePhysicsHandler::update_gravity(EntityId entity_id) {
    for (auto rectangle_id : sys_entities) {
        auto rectangle_body = global->ECS.get_component<Body>(rectangle_id);
        auto entity_body = global->ECS.get_component<Body>(entity_id);
        auto rect_shape = global->ECS.get_component<RectangleShape>(rectangle_id);
        // TODO: change the Y axis of the player to match
        // the side of the rectangle we are on vertically
    }
}
