#ifndef PLAYER_COLLISIONS_HPP
#define PLAYER_COLLISIONS_HPP
#include "fumo_engine/core/system_base.hpp"
#include "fumo_engine/components.hpp"

// both used to check collisions for the player agaisnt the planets
class PlayerCollisionRunner : public System {
    void sys_call() override { check_collisions(); }
    void check_collisions();
    bool player_to_rect_collision_solving(PlayerShape& player_shape, Body& player_body,
                                          const Rectangle& rectangle,
                                          const Body& rectangle_body);
    bool player_to_circle_collision_solving(PlayerShape& player_shape, Body& player_body,
                                            const Circle& circle_shape,
                                            const Body& circle_body);
};
#endif
