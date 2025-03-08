#ifndef PLAYER_COLLISIONS_HPP
#define PLAYER_COLLISIONS_HPP
#include "fumo_engine/core/engine_constants.hpp"
#include "fumo_engine/core/system_base.hpp"
#include "objects/components.hpp"

// both used to check collisions for the player agaisnt the planets

class CircleCollisionHandler : public System {
  public:
    void sys_call() override {}
    void check_collisions_with_player();
    void check_collision_with_player(EntityId circle_entity_id, EntityId entity_id);
    // void check_collisions_with_rect(EntityId entity_id);
    void solve_collision_player(Body& body, Body& other_body, Circle& shape,
                                Circle& other_shape);
};

class PlayerCollisionRunner : public System {
    void sys_call() override { check_collisions(); }
    void check_collisions();
    void player_to_rect_collision_solving(PlayerShape& player_shape, Body& player_body,
                                          const Rectangle& rectangle,
                                          const Body& rectangle_body);
    void player_to_circle_collision_solving(PlayerShape& player_shape, Body& player_body,
                                            const Circle& circle_shape,
                                            const Body& circle_body);
};
#endif
