
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/core/scheduling_systems.hpp"
#include "fumo_engine/collisions_and_physics/gravity_physics.hpp"

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
