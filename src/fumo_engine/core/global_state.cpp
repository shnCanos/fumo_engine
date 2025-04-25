#include "fumo_engine/core/global_state.hpp"

#include "objects/player_systems/player_general_systems.hpp"

namespace Initialization {
void initialize_directories();

}

void FumoEngine::setup_game_state() {
    SetTargetFPS(60);

    Initialization::initialize_directories();
    player_id = PlayerInitializer::initialize_player();

    camera = std::make_unique<Camera2D>();
    const auto& player_body = ECS->get_component<Body>(player_id);
    // camera->target = player_body.position;
    camera->target = screenCenter.to_raylib_vec2();
    camera->rotation = 0.0f;
    camera->zoom = 0.8f;
    camera->offset = screenCenter.to_raylib_vec2();
    auto& player_animation = ECS->get_component<AnimationInfo>(player_id);
    AnimationPlayer::play(player_animation, "idle");
}

namespace fs = std::filesystem;

// assume we have something like:
// build/
// | game_executable
// assets/
// serialized_data/
void Initialization::initialize_directories() {
    // TODO: only works on linux (add windows support later)
    fs::path curr_path = fs::current_path();

    if (*--curr_path.end() == "build") {
        // go up a dir name
        fs::current_path(curr_path.parent_path());
    }
    fs::create_directory("serialized_data");

    curr_path = fs::current_path();
    fs::current_path(curr_path / "serialized_data");
}
