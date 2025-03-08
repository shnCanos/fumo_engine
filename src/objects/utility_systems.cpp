
#include "fumo_engine/core/global_state.hpp"
#include "systems.hpp"

extern std::unique_ptr<GlobalState> global;
void Debugger::global_debug() {
    // NOTE: only debug printing the ECS->structure atm (not the global state)
    global->ECS->debug_print();
}

// void Initializer::awake_systems() {
//     auto input_handler_ptr = global->ECS->get_system<InputHandlerLevelEditor>();
//     input_handler_ptr->awake = true;
// }
