#include "fumo_engine/global_state.hpp"
#include "objects/systems.hpp"
#include "raylib.h"

extern std::unique_ptr<GlobalState> global;

void PlayerInputHandler::handle_input() {
    const auto body_movement_ptr = global->ECS->get_system<BodyMovement>();
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    if (IsKeyDown(KEY_SPACE)) {
        if (player_body.touching_ground) {
            body_movement_ptr->jump(player_body);
        }
    }
    if (IsKeyDown(KEY_DOWN)) {
        body_movement_ptr->move_vertically(player_body, -1.0f);
    }
    if (IsKeyDown(KEY_UP)) {
        body_movement_ptr->move_vertically(player_body, 1.0f);
    }
    if (IsKeyDown(KEY_LEFT)) {
        body_movement_ptr->move_horizontally(player_body, -1.0f);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        body_movement_ptr->move_horizontally(player_body, 1.0f);
    }
    body_movement_ptr->update_position(player_body);
}
