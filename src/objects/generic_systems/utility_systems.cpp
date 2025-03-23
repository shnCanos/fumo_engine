
#include "fumo_engine/core/global_state.hpp"
#include "systems.hpp"

extern std::unique_ptr<GlobalState> global;
void Debugger::global_debug() { global->ECS->debug_print(); }
