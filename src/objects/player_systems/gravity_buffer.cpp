
#include "fumo_engine/global_state.hpp"
#include "fumo_engine/scheduling_systems.hpp"
#include "objects/player_systems/player_physics.hpp"

extern std::unique_ptr<GlobalState> global;

void GravityBufferHandler::wait_for_touching_ground() {

    const auto& body = global->ECS->get_component<Body>(global->player_id);

    if (body.touching_ground) {
        const auto& scheduler_system = global->ECS->get_system<SchedulerSystemECS>();
        scheduler_system->awake_system<GravityHandler>();
    }
}
