#include "fumo_engine/entity_query.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"
#include "objects/systems.hpp"

extern std::unique_ptr<GlobalState> global;

void register_components();
void register_systems();
void create_entities();

void register_all_to_ECS() {
    register_components();
    register_systems();
    create_entities();
}

void register_components() {
    global->ECS.register_component<Body>();
    global->ECS.register_component<Render>();
    global->ECS.register_component<CircleShape>();
}
void register_systems() {
    global->ECS.register_system_unscheduled<BodyMovement>(
        EntityQuery{.component_mask = 0, .component_filter = Filter::Only});

    global->ECS.register_system<InputHandlerLevelEditor, 0>(EntityQuery{
        .component_mask = global->ECS.make_component_mask<Body, Render, CircleShape>(),
        .component_filter = Filter::Only});

    global->ECS.register_system<PlanetRenderer, 1>(EntityQuery{
        .component_mask = global->ECS.make_component_mask<Body, Render, CircleShape>(),
        .component_filter = Filter::Only});

    global->ECS.register_system_unscheduled<PlanetFactory>(
        EntityQuery{.component_mask = 0, .component_filter = Filter::Only});

    // global->ECS.register_system<CirclePhysicsUpdate, 2>(EntityQuery{
    //     .component_mask = global->ECS.make_component_mask<Body, Render, CircleShape>(),
    //     .component_filter = Filter::Only});
}
void create_entities() {}

