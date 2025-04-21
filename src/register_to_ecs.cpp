#include "fumo_engine/core/global_state.hpp"
#include "include_systems.hpp"

extern std::unique_ptr<GlobalState> global;

void register_components();
void register_systems();
void register_unregistered_systems_unscheduled();
void register_systems_scheduled();

void register_all_to_ECS() {
    register_components();
    register_systems();
}

// Body
// Circle
// AnimationInfo
// Timer
// Render
// FumoRect
// PlayerShape
// ParallelGravityField
// CircularGravityField
// ColliderObjectFlag
// GravFieldFlag
// OutlineRectFlag
// EntityState
// MovedEventData

void register_components() {
    global->ECS->register_component<Body>();
    global->ECS->register_component<Circle>();
    global->ECS->register_component<AnimationInfo>();
    global->ECS->register_component<Timer>();
    global->ECS->register_component<Render>();
    global->ECS->register_component<FumoRect>();
    global->ECS->register_component<PlayerShape>();
    global->ECS->register_component<ParallelGravityField>();
    global->ECS->register_component<CircularGravityField>();
    global->ECS->register_component<ColliderObjectFlag>();
    global->ECS->register_component<GravFieldFlag>();
    global->ECS->register_component<OutlineRectFlag>();
    global->ECS->register_component<EntityState>();
    global->ECS->register_component<MovedEventData>();
    global->ECS->register_component<Screen>();
    global->ECS->register_component<LevelId>();

    // new
    // ------
    global->ECS->register_component<ScreenTransitionRect>();

    // global->ECS->register_component<OnScreen>();
}

void register_systems() {
    register_unregistered_systems_unscheduled();
    register_systems_scheduled();
}

void register_systems_scheduled() {
    //--------------------------------------------------------------------------------------
    // actual game logic
    global->ECS->add_unregistered_system<PlayerInputHandler, 0>();
    // gravity updater is a registered system,
    // but its only running on the player right now
    global->ECS->add_unregistered_system<GravityUpdater, 2>();
    global->ECS->register_system_unscheduled<LevelSerializer>(EntityQuery {
        .component_mask = global->ECS->make_component_mask<LevelId, Screen>(),
        .component_filter = Filter::All});

    global->ECS->register_system<GravityFieldHandler, 3>(EntityQuery {
        .component_mask = global->ECS->make_component_mask<GravFieldFlag>(),
        .component_filter = Filter::All});

    global->ECS->register_system<PlayerCollisionRunner, 4>(
        EntityQuery {.component_mask =
                         global->ECS->make_component_mask<ColliderObjectFlag>(),
                     .component_filter = Filter::Any});
    global->ECS->register_system<ScreenTransitionHandler, 5>(EntityQuery {
        .component_mask =
            global->ECS->make_component_mask<Body, ScreenTransitionRect>(),
        .component_filter = Filter::All});

    global->ECS->register_system<StateHandler, MAX_PRIORITY - 1>(EntityQuery {
        .component_mask = global->ECS->make_component_mask<EntityState>(),
        .component_filter = Filter::All});
    //--------------------------------------------------------------------------------------
    // misc systems
    global->ECS->register_system<DebugLevelEditor, 5>(EntityQuery {
        .component_mask =
            global->ECS
                ->make_component_mask<GravFieldFlag, ColliderObjectFlag>(),
        .component_filter = Filter::Any});
    // global->ECS->register_system<TimerHandler, 7>(
    //     EntityQuery{.component_mask = global->ECS->make_component_mask<Timer>(),
    // .component_filter = Filter::Only});

    //--------------------------------------------------------------------------------------
    // render everything at the end
    global->ECS->register_system<ObjectRenderer, MAX_PRIORITY>(
        EntityQuery {.component_mask =
                         global->ECS->make_component_mask<ColliderObjectFlag>(),
                     .component_filter = Filter::All});

    global->ECS->register_system<AnimationRenderer, MAX_PRIORITY>(EntityQuery {
        .component_mask =
            global->ECS->make_component_mask<Body, AnimationInfo>(),
        .component_filter = Filter::All});

    global->ECS->register_system<GravFieldRenderer, MAX_PRIORITY>(EntityQuery {
        .component_mask = global->ECS->make_component_mask<ParallelGravityField,
                                                           Render,
                                                           Body,
                                                           GravFieldFlag>(),
        .component_filter = Filter::All});
    //--------------------------------------------------------------------------------------
}

void register_unregistered_systems_unscheduled() {
    global->ECS->add_unregistered_system_unscheduled<SchedulerSystemECS>(
        global->ECS);
    // global->ECS->add_unregistered_system_unscheduled<GravityBufferHandler>();
    // global->ECS->add_unregistered_system_unscheduled<EntireAnimationPlayer>();
    global->ECS->add_unregistered_system_unscheduled<LevelEntityFactory>();
    // FIXME: dont register delegate event wrappers
    // otherwise, there can only be one entity moving per frame
    // create a new stack object, and it will get destroyed along with
    // the popped event that contains it
    // global->ECS->add_unregistered_system_unscheduled<MovedWrapper>();
}
