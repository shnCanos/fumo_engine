// #include "constants.hpp"
// #include "fumo_ecs/constants_using.hpp"
// #include "fumo_ecs/entity_query.hpp"
// #include "fumo_ecs/global_state.hpp"
// #include "old_objects/components.hpp"
// #include "old_objects/systems.hpp"
// #include "raylib.h"
// extern std::unique_ptr<GlobalState> global;
// EntityId create_celestial_body(float radius, float mass, Vector2 velocity,
//                                Vector2 position, Color color);
// void make_lots_of_entities_test();
// void ecs_test() {
//     global->ECS.register_component<Body>();
//     global->ECS.register_component<Render>();
//     global->ECS.register_component<CircleShape>();
//
//     // added component mask methods (takes any component type)
//     global->ECS.register_system<CirclePhysicsUpdate, 0>(EntityQuery{
//         .component_mask = global->ECS.make_component_mask<Body, Render, CircleShape>(),
//         .component_filter = Filter::All});
//     global->ECS.register_system<class DrawCircle, 1>(EntityQuery{
//         .component_mask = global->ECS.make_component_mask<Body, Render, CircleShape>(),
//         .component_filter = Filter::All});
//
//     make_lots_of_entities_test();
// }
//
// void add_an_entity() {
//     create_celestial_body(10, 6000, {0, 140},
//                           {screenWidth / 2.0f, 3.0f * screenHeight / 4.0f}, PURPLE);
// }
// void make_lots_of_entities_test() {
//     create_celestial_body(10, 6000, {0, 140},
//                           {screenWidth / 2.0f, 3.0f * screenHeight / 4.0f}, ORANGE);
//     create_celestial_body(24, 60000, {0, 140}, {screenWidth / 4.0f, screenHeight / 2.0f},
//                           BLUE);
//     create_celestial_body(50, 6000000, {100, 140},
//                           {3.0f * screenWidth / 4.0f, screenHeight / 2.0f}, GREEN);
//     create_celestial_body(100, 89000000, {0, 0}, screenCenter, RED);
// }
//
// EntityId create_celestial_body(float radius, float mass, Vector2 velocity,
//                                Vector2 position, Color color) {
//     EntityId entity_id = global->ECS.create_entity();
//     global->ECS.entity_add_component(
//         entity_id, Body{.position = position, .velocity = velocity, .mass = mass});
//     global->ECS.entity_add_component(entity_id, Render{.color = color});
//     global->ECS.entity_add_component(entity_id, CircleShape{.radius = radius});
//     return entity_id;
// }
// // old comments, might delete, seems to be solved
// // cant register the systems effectively until ive made a system
// // that allows me to pass to them the variables i want for their function.
// // this means that a good solution is treating them the same as components
// // and require an initialization like a component requires
// // and i will now copy over the components code for systems
// // and then maybe later we can make system entities and separate both logics
// // and have a nice finished system.
