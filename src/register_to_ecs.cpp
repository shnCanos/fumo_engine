#include "fumo_engine/global_state.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"
#include "objects/systems.hpp"
// angular include so clang wont complain
#include <include_systems.hpp>

extern std::unique_ptr<GlobalState> global;

void register_components();
void register_systems();
void register_systems_physics_collisions();
void register_agnostic_sytems();
void register_systems_scheduled();
void create_entities();
void register_all_to_ECS() {
    register_components();
    register_systems();
    create_entities();
}

void register_components() {
    global->ECS->register_component<Body>();
    global->ECS->register_component<CircleShape>();
    global->ECS->register_component<GravityField>();
    global->ECS->register_component<PlayerFlag>();
    global->ECS->register_component<AnimationInfo>();
    global->ECS->register_component<Timer>();
    global->ECS->register_component<Render>();
    global->ECS->register_component<AggregateField>();
}
void register_systems() {
    // NOTE: consider how you would stop systems from running based on
    // conditions or the game state (unregister the system?)
    register_agnostic_sytems();
    register_systems_physics_collisions();
    register_systems_scheduled();
}
void register_systems_scheduled() {

    global->ECS->add_unregistered_system<PlayerInputHandler, 0>();

    global->ECS->register_system<InputHandlerLevelEditor, 1>(EntityQuery{
        .component_mask =
            global->ECS->make_component_mask<Body, Render, CircleShape>(),
        .component_filter = Filter::All});

    global->ECS->register_system<GravityHandler, 2>(EntityQuery{
        .component_mask =
            global->ECS->make_component_mask<Body, CircleShape, GravityField>(),
        .component_filter = Filter::All});

    global->ECS->add_unregistered_system<GravityUpdater, 3>();

    global->ECS->add_unregistered_system<PlayerCollisionRunner, 5>();

    global->ECS->register_system<PlanetRenderer, 6>(EntityQuery{
        .component_mask =
            global->ECS->make_component_mask<Body, Render, CircleShape>(),
        .component_filter = Filter::All});

    global->ECS->register_system<TimerHandler, 7>(
        EntityQuery{.component_mask = global->ECS->make_component_mask<Timer>(),
                    .component_filter = Filter::Only});

    global->ECS->register_system<AnimationRenderer, 60>(EntityQuery{
        .component_mask = global->ECS->make_component_mask<Body, AnimationInfo>(),
        .component_filter = Filter::All});

    global->ECS->add_unregistered_system<PlayerEndFrameUpdater, MAX_PRIORITY - 1>();
}

void register_systems_physics_collisions() {
    global->ECS->register_system_unscheduled<CircleCollisionHandler>(EntityQuery{
        .component_mask =
            global->ECS->make_component_mask<Body, CircleShape>(),
        .component_filter = Filter::All});
    // global->ECS->register_system_unscheduled<CirclePhysicsHandler>(EntityQuery{
    //     .component_mask =
    //         global->ECS->make_component_mask<Body, CircleShape, GravityField>(),
    //     .component_filter = Filter::All});
}
void register_agnostic_sytems() {
    global->ECS->add_unregistered_system_unscheduled<SchedulerSystemECS>(global->ECS);
    global->ECS->add_unregistered_system_unscheduled<AnimationPlayer>();
    global->ECS->add_unregistered_system_unscheduled<GravityBufferHandler>();
    global->ECS->add_unregistered_system_unscheduled<JumpPhysicsHandler>();
    global->ECS->add_unregistered_system_unscheduled<PlayerInitializer>();
    global->ECS->add_unregistered_system_unscheduled<EntireAnimationPlayer>();
    global->ECS->add_unregistered_system_unscheduled<BodyMovement>();
    global->ECS->add_unregistered_system_unscheduled<PlanetFactory>();
    global->ECS->add_unregistered_system_unscheduled<Debugger>();
}

void create_entities() {}
