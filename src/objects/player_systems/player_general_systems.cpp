#include "objects/player_systems/player_general_systems.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "objects/components.hpp"

extern std::unique_ptr<GlobalState> global;
void debug_player_drawing(const PlayerShape& player_shape, const Body& player_body);

EntityId PlayerInitializer::initialize_player() {

    EntityId player_id = global->ECS->create_entity();

    global->ECS->entity_add_component(player_id, PlayerFlag{.can_swap_orbits = true});
    global->ECS->entity_add_component(player_id, AnimationInfo{.sprite_scaling = 3.0f});
    global->ECS->entity_add_component(player_id, Render{.color = {50, 50, 50, 150}});

    Body player_body{
        .position = screenCenter, .velocity = {0.0f, 0.0f}};
    global->ECS->entity_add_component(player_id, player_body);

    PlayerShape player_shape{.radius = 33.0f};
    player_shape.update_capsule_positions(player_body);
    global->ECS->entity_add_component(player_id, player_shape);

    return player_id;
}

void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player) {
    float minSpeed = 30;
    float minEffectLength = 10;
    float fractionSpeed = 0.8f;

    camera->offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    Vector2 diff = Vector2Subtract(player.position, camera->target);
    float length = Vector2Length(diff);

    float scaling = 8.0f;

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target =
            Vector2Add(camera->target,
                       Vector2Scale(diff, speed * global->frametime / length) * scaling);
    }
}

void PlayerEndFrameUpdater::reset_state() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
}

void PlayerEndFrameUpdater::end_of_frame_update() {

    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);

    // camera follows player
    UpdateCameraCenterSmoothFollow(global->camera.get(), player_body);

    //-----------------------------------------------------------------
    // apply movement changes to the player
    player_body.x_direction = {player_body.gravity_direction.y,
                               -player_body.gravity_direction.x};
    player_body.rotation =
        std::atan2(player_body.x_direction.y, player_body.x_direction.x) * RAD2DEG;

    player_body.position += player_body.velocity * global->frametime;
    player_shape.update_capsule_positions(player_body);

    //-----------------------------------------------------------------
    // DEBUG
    debug_player_drawing(player_shape, player_body);
    //-----------------------------------------------------------------

    player_body.velocity = {0.0f, 0.0f};
}

void debug_player_drawing(const PlayerShape& player_shape, const Body& player_body) {

    // extra visualization code
    BeginMode2D(*global->camera);
    const auto& render = global->ECS->get_component<Render>(global->player_id);
    DrawCircleV(player_shape.bottom_circle_center, player_shape.radius, render.color);
    DrawCircleV(player_shape.top_circle_center, player_shape.radius, render.color);

    // double gravity_reach = 300.0f;
    // Vector2 normalized_velocity = Vector2Normalize(player_body.velocity);
    // Vector2 line_end = player_body.position + normalized_velocity * gravity_reach;
    // if (player_body.jumping) {
    //     line_end =
    //         player_body.position - (player_body.gravity_direction) * gravity_reach;
    // }
    // DrawLineV(player_body.position, line_end, YELLOW);
    EndMode2D();
}
