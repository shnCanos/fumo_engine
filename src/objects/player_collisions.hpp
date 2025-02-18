#ifndef PLAYER_COLLISIONS_HPP
#define PLAYER_COLLISIONS_HPP
#include "fumo_engine/engine_constants.hpp"
#include "fumo_engine/system_base.hpp"
#include "objects/components.hpp"

// both used to check collisions for the player agaisnt the planets

class CircleCollisionHandler : System {
  public:
    void check_collisions_with_player();
    void check_collision_with_player(EntityId circle_entity_id, EntityId entity_id);
    // void check_collisions_with_rect(EntityId entity_id);
    void solve_collision_player(Body& body, Body& other_body, CircleShape& shape,
                                CircleShape& other_shape);
};

class RectangleCollisionHandler : System {
  public:
    void check_collisions_with_circle(EntityId entity_id);
    void check_collisions_with_rect(EntityId entity_id);
    void solve_collision(EntityId entity_id);
};

class PlayerCollisionRunner : System {
    // private:
    //   std::shared_ptr<CircleCollisionHandler> circle_collision_handler;
    //   std::shared_ptr<RectangleCollisionHandler> rect_collision_handler;

    // NOTE: i have concluded that my ECS is indeed good, and i think it should
    // scale well when adding more systems.
    // note that we make system "runners" that hold pointers to systems
    // to take generic systems and make more meaningful uses of them
    // (its the equivalent of ordering them in the big global scheduler, but doing
    // it step by step by storing them inside these system "runners")
  public:
    void sys_call() override {};

    void check_collisions();
    void update_position();
    void update_velocity();
};
#endif
