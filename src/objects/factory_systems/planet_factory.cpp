#include "fumo_engine/core/global_state.hpp"
#include "objects/factory_systems/factory_systems.hpp"

extern std::unique_ptr<FumoEngine> fumo_engine;

void LevelEntityFactory::delete_planet(EntityId entity_id) {
    // here to keep track of what this instance of planet factory
    // has created so we can isolate these entities later
    // useful for grouping up entities based on their used context
    fumo_engine->ECS->destroy_entity(entity_id);
    sys_entities.erase(entity_id);
    // TODO: create a player_owning component,
    // and have gravity updater query for that instead.
    // this is way too coupled by saving it outside of the ECS
    // like we are doing right now
    // const auto& gravity_updater = fumo_engine->ECS->get_system<GravityUpdater>();
    auto& player_state =
        fumo_engine->ECS->get_component<EntityState>(fumo_engine->player_id);
    if (player_state.player_owning_field == entity_id) {
        player_state.player_owning_field = NO_ENTITY_FOUND;
    }
}

void LevelEntityFactory::delete_all_planets() {
    for (EntityId entity_id : sys_entities) {
        fumo_engine->ECS->destroy_entity(entity_id);
    }
    sys_entities.clear();
}
