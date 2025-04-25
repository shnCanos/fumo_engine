#include "fumo_engine/components.hpp"
#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;
const static EntityQuery Circle_query {};
const static EntityQuery CircularGravityField_query {};
const static EntityQuery FumoRect_query {};
const static EntityQuery ParallelGravityField_query {};
const static EntityQuery OutlinneRectFlag_query {};

void func(const EntityId& entity_id) {
    if (global->ECS->filter(entity_id, Circle_query)) {
        const auto& circle_shape =
            global->ECS->get_component<Circle>(entity_id);
    }
}
