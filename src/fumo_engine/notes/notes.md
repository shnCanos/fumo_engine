# Notes for fumo engine level editor



## main features we want to implement into fumo engine:
### adding more event functionality 
- (we are missing the coyotte time for the jump)
- we are missing the event for swapping orbits that makes the transitioning smoother


### level editor
is responsible for storing the serialized level data
and distributing it into the ECS accordingly
also moves forward the level Tree/Linked list forward
and keeps track of where we are, etc



- how do i organize levels data in the engine ???
> ????? 
> 


since it would be more optimal in the future
we will only load in each level into the ECS at a time.
possibly, we only load in the screen we are on and the adjacent screens too
but i think the adding and removing is largely unnecesary. mayhaps we load half?
this kind of behavior would be easier to change later if we put all the loading
behavior in one single place. this way we should easily be able to only load 
in half of the level in case this is either desired or at least more optimized
than if we didnt do this kind of approach

- quick note:
i think a better general approach to a new complex system,
is to **really isolate what it wants to do**
i think checking for level transitions can be done by itself
and the logic for what should happen doesnt necessarily need to happen
in the level manager class if it can be avoided
turning it into an event can help isolate the behavior a lot.
on the other hand, a lot of the behavior we are looking for might involve
a lot of changing OF the level manager's data itself. if this is the case
then there might not be a reason to change stuff in an event, and we can do it
in the level manager itself. 
however
to change screens we:
change camera position to the new screen location
possibly freeze the game while smoothly scrolling the camera
possibly glue the player to the ground somehow on some transitions
etc.

//
// #include "fumo_engine/core/system_base.hpp"
// #include "fumo_raylib.hpp"
//
// // if we collide with it, we change the screen we are on
// struct ScreenTransitionRect {};
//
// // works the exact same way as an orbit transition:
// // - wait for the screen to finish transitioning before we unfreeze the game
// // - only when we leave the range of the screen transition
// //   do we allow the player to transition again back to the previous screen
//
// struct Level {};
//
// // level object positions are always set by the following:
// // - top left of the screen is {0,0} relative to the objects
// // - when we draw/place them in the world, we add the screen top left
// struct Screen {
//     FumoVec2 screen_position; // screen top left position
// };
//
// // struct OnScreen{};
// using ScreenId = uint64_t;
// constexpr ScreenId LEVEL_COUNT = 64;
//
// struct LevelManager: public System {
//     // sends out a screen transition event
//     // we change camera position and other stuff
//
//     std::array<ScreenId, LEVEL_COUNT> all_level_ids {};
//
//     void sys_call() override {
//         check_for_screen_transition();
//     }
//
//     void check_for_screen_transition();
//
//     void load_level_to_ECS();
// };
//
// // need to somehow associate the objects to each screen/level
// // and it needs to also work with the serialization of levels
// // do we save what level it WAS on? am i overthinking the complexity
// // of properly structuring serialized data in a game?
// // TODO:
// // > look at godot's Screen class and see if that helps with what im writing
//
// struct LevelEditorGUI {
//     void save_level();
//     // ... add new level editor functionality here
//     // or copy it from the current level editor input class
// };
