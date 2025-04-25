#include "fumo_engine/core/entity_query.hpp"
// very useful for generating code related to our components
// XMACRO_ACTION(Type) can be redefined to generate code with each Type
#define ALL_COMPONENTS_X_MACRO() \
    XMACRO_ACTION(Body) \
    XMACRO_ACTION(Circle) \
    XMACRO_ACTION(AnimationInfo) \
    XMACRO_ACTION(Timer) \
    XMACRO_ACTION(Render) \
    XMACRO_ACTION(FumoRect) \
    XMACRO_ACTION(Capsule) \
    XMACRO_ACTION(ParallelGravityField) \
    XMACRO_ACTION(CircularGravityField) \
    XMACRO_ACTION(ColliderObjectFlag) \
    XMACRO_ACTION(GravFieldFlag) \
    XMACRO_ACTION(OutlineRectFlag) \
    XMACRO_ACTION(EntityState) \
    XMACRO_ACTION(MovedEventData) \
    XMACRO_ACTION(Screen) \
    XMACRO_ACTION(LevelId) \
    XMACRO_ACTION(ScreenTransitionRect) \
    XMACRO_ACTION(Line) \
    XMACRO_ACTION(CollisionEventData)

#define XMACRO_ACTION(Type) Type,
enum struct AllComponentTypes { ALL_COMPONENTS_X_MACRO() };
#undef XMACRO_ACTION

#define XMACRO_ACTION(Type) static EntityQuery Type##_query {};
ALL_COMPONENTS_X_MACRO()
#undef XMACRO_ACTION
