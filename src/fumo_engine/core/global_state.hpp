#pragma once
#include <memory>

#include "fumo_engine/components.hpp" // IWYU pragma: export
#include "constants/engine_constants.hpp"
#include "fumo_engine/core/scheduler_ecs.hpp"
#include "fumo_engine/events/event_state_handlers.hpp"
#include "fumo_engine/sprite_animation_manager/sprite_and_animation_systems.hpp"


class GlobalState {
  public:
    float frametime;
    std::shared_ptr<SchedulerECS> ECS;

    std::unique_ptr<SpriteManager> sprite_manager;

    std::unique_ptr<Camera2D> camera;

    std::unique_ptr<EntityEventHandler> event_handler;


    EntityId
        player_id; // NOTE: storing the player id globally for now for optimisation

    // this isnt necessary, and can be removed completely later.

    void initialize() {
        ECS = std::make_shared<SchedulerECS>();
        ECS->initialize();
        sprite_manager = std::make_unique<SpriteManager>();
        event_handler = std::make_unique<EntityEventHandler>();
    }

    void handle_events() {
        event_handler->handle_events();
    }

    void setup_game_state();

    void destroy_and_unload_game() {
        // TODO: unload our textures and also destroy all dynamic memory in global
        // (so far we have no memory to clear so this is not necessary)
    }
};
