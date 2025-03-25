#ifndef FLAG_COMPONENTS_HPP
#define FLAG_COMPONENTS_HPP
#include <memory>

#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/system_base.hpp"
#include "objects/generic_systems/systems.hpp"
#include "raylib.h"

// slowly rotate player to the new gravity direction
// change controls like mario galaxy (relative to the screen)
//
// keep old movement when swapping orbits
// -> separate it from all inputs after swapping
// -> (once we RELEASE arrow keys, we are now swapped)
// --> before that, we ignore the new direction
constexpr uint64_t MAX_EVENTS = 64;
//-----------------------------------------------------------------
// state and event components
enum struct EVENT_ {
    //-----------------------------------------------------------------
    // input handling
    ENTITY_JUMPED,
    ENTITY_MOVED,
    ENTITY_IDLE,
    //-----------------------------------------------------------------
    ENTITY_COLLIDED,
    //-----------------------------------------------------------------
    ENTITY_SWAPPED_ORBITS,
    // ENTITY_FELL_FROM_GROUND,
};

struct Event {
    EVENT_ event;
    EntityId entity_id;
    std::shared_ptr<System> delegate_system;
};

struct EntityState {
    bool on_ground = true;
    bool jumping = false;
    bool can_swap_orbits = true;
    bool can_jump = true;
    bool idle = true;

    EntityId player_owning_field = NO_ENTITY_FOUND;
};

struct MovedEventData {
    DIRECTION direction;
    DIRECTION previous_direction = DIRECTION::NO_DIRECTION;
    DIRECTION continue_in_direction;
};

//-----------------------------------------------------------------
// flag structs
struct Level1Tag {
    // identify objects in level 1
    // NOTE: general idea for how levels work:
    //
    // we create all the entities a level will need, then we store them in some
    // level manager thing that deals with them, and knows what belongs where
    // it also adds the OnScreen component to entities so other systems
    // can know what entities they should be updating
    // and it manages the screen transitions
};

struct OnScreen {
    // given to entities that are currently on screen
    // (used by systems that only care about what should be on the screen)
};

struct Render {
    Color color;
};

// struct PlayerFlag {
//     // used to identify the player uniquely by associating this struct
//     // to an entity id
// };

struct ColliderObjectFlag {
    // identifies planets and rectangles and such shapes
};

struct GravFieldFlag {
    // if it has a field of any type
};

struct OutlineRectFlag {};

#endif
