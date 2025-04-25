#include "fumo_engine/all_components_macro.hpp"
#include "fumo_engine/core/global_state.hpp"
#include "include_systems.hpp"
extern std::unique_ptr<FumoEngine> fumo_engine;

void register_components();
void register_systems();
void register_unregistered_systems_unscheduled();
void register_systems_scheduled();

void register_all_to_ECS() {
    register_components();
    register_systems();
}

void register_components() {

// also creates the component's corresponding entity_query globally
#define XMACRO(Type) \
    extern EntityQuery Type##_query; \
    fumo_engine->ECS->register_component<Type>(); \
    Type##_query = {.component_mask = \
                        fumo_engine->ECS->make_component_mask<Type>(), \
                    .component_filter = Filter::All};

    // calls the macro
    ALL_COMPONENTS_X_MACRO()
// undef so we can use it again later
#undef XMACRO
}

void register_systems() {
    register_unregistered_systems_unscheduled();
    register_systems_scheduled();
}

void register_systems_scheduled() {
    //--------------------------------------------------------------------------------------
    // actual game logic
    fumo_engine->ECS->add_unregistered_system<PlayerInputHandler, 0>();
    // gravity updater is a registered system,
    // but its only running on the player right now
    fumo_engine->ECS->add_unregistered_system<GravityUpdater, 2>();
    fumo_engine->ECS->register_system_unscheduled<LevelSerializer>(EntityQuery {
        .component_mask =
            fumo_engine->ECS->make_component_mask<LevelId, Screen>(),
        .component_filter = Filter::All});

    fumo_engine->ECS->register_system<GravityFieldHandler, 3>(
        EntityQuery {.component_mask =
                         fumo_engine->ECS->make_component_mask<GravFieldFlag>(),
                     .component_filter = Filter::All});

    fumo_engine->ECS->register_system<CollisionRunner, MAX_PRIORITY - 1>(
        EntityQuery {
            .component_mask =
                fumo_engine->ECS->make_component_mask<ColliderObjectFlag>(),
            .component_filter = Filter::Any});
    fumo_engine->ECS->register_system<ScreenTransitionHandler, 5>(EntityQuery {
        .component_mask =
            fumo_engine->ECS->make_component_mask<Body, ScreenTransitionRect>(),
        .component_filter = Filter::All});

    fumo_engine->ECS->register_system<StateHandler, MAX_PRIORITY - 2>(
        EntityQuery {.component_mask =
                         fumo_engine->ECS->make_component_mask<EntityState>(),
                     .component_filter = Filter::All});
    //--------------------------------------------------------------------------------------
    // misc systems
    fumo_engine->ECS->register_system<DebugLevelEditor, 5>(EntityQuery {
        .component_mask =
            fumo_engine->ECS
                ->make_component_mask<GravFieldFlag, ColliderObjectFlag>(),
        .component_filter = Filter::Any});
    // fumo_engine->ECS->register_system<TimerHandler, 7>(
    //     EntityQuery{.component_mask = fumo_engine->ECS->make_component_mask<Timer>(),
    // .component_filter = Filter::Only});

    //--------------------------------------------------------------------------------------
    // render everything at the end
    fumo_engine->ECS->register_system<ComponentRenderer, MAX_PRIORITY>(
        EntityQuery {.component_mask =
                         fumo_engine->ECS->make_component_mask<Body, Render>(),
                     .component_filter = Filter::All});

    fumo_engine->ECS->register_system<AnimationRenderer, MAX_PRIORITY>(
        EntityQuery {
            .component_mask =
                fumo_engine->ECS->make_component_mask<Body, AnimationInfo>(),
            .component_filter = Filter::All});

    // fumo_engine->ECS->register_system<GravFieldRenderer, MAX_PRIORITY>(EntityQuery {
    //     .component_mask = fumo_engine->ECS->make_component_mask<ParallelGravityField,
    //                                                        Render,
    //                                                        Body,
    //                                                        GravFieldFlag>(),
    //     .component_filter = Filter::All});
    //--------------------------------------------------------------------------------------
}

void register_unregistered_systems_unscheduled() {
    fumo_engine->ECS->add_unregistered_system_unscheduled<SchedulerSystemECS>(
        fumo_engine->ECS);
    // fumo_engine->ECS->add_unregistered_system_unscheduled<GravityBufferHandler>();
    // fumo_engine->ECS->add_unregistered_system_unscheduled<EntireAnimationPlayer>();
    fumo_engine->ECS->add_unregistered_system_unscheduled<LevelEntityFactory>();
    // FIXME: dont register delegate event wrappers
    // otherwise, there can only be one entity moving per frame
    // create a new stack object, and it will get destroyed along with
    // the popped event that contains it
    // fumo_engine->ECS->add_unregistered_system_unscheduled<MovedWrapper>();
}
