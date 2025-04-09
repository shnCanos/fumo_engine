#include "fumo_engine/level_systems/fumo_serializer.hpp"

#include <fstream>

#include "filesystem"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/level_systems/fumo_serializer.hpp"

extern std::unique_ptr<GlobalState> global;
// NOTE: maybe switch this to a namespace later if this class is useless

namespace fs = std::filesystem;

void LevelSerializer::serialize_levels() {
    for (const auto& entity_id : sys_entities) {
        const auto& screen_id = global->ECS->get_component<ScreenId>(entity_id);
        const auto& level_id = global->ECS->get_component<LevelId>(entity_id);

        fs::create_directory(std::format("level{}", level_id.level_id));

        // FIXME: add the directory to the file path later (with std::filesystem::path)
        std::ofstream out_stream(std::format("screen{}.json", screen_id.screen_id));

        if (out_stream.fail()) [[unlikely]]
            PANIC("failed to open or create file\n");

        FumoSerializer::serialize_entity(entity_id,
                                         cereal::JSONOutputArchive(out_stream));
    }
};

void FumoSerializer::serialize_entity(const EntityId& entity_id,
                                      const cereal::JSONOutputArchive& out_archive) {
    const auto& component_mask = global->ECS->get_component_mask(entity_id);

    for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
        if (component_mask & (1 << id)) {
            global->ECS->serialize_component(entity_id, id, out_archive);
        }
    }
}

void FumoSerializer::deserialize_entity(const EntityId& entity_id,
                                        cereal::JSONInputArchive& in_archive) {
    // how will we deserialize the information?
    // example of the JSON:
    // 0 corresponds to the Body struct component id when registered
    // {
    //     "entity_id1": 1,
    //     "0": {
    //         "position": {
    //             "x": 69.0,
    //             "y": 6969.0
    //         }
    //     },
    // }
    // how do i find out what the type is again? std::variant....?
}

#define DESERIALIZE_COMPONENT(Type) \
    global->ECS->entity_add_component( \
        entity_id, \
        FumoSerializer::deserialize_component<Type>(component_id, in_archive))

enum struct AllComponentTypes {
    Body,
    Circle,
    AnimationInfo,
    Timer,
    Render,
    Rectangle,
    PlayerShape,
    ParallelGravityField,
    CircularGravityField,
    ColliderObjectFlag,
    GravFieldFlag,
    OutlineRectFlag,
    EntityState,
    MovedEventData,
};

void FumoSerializer::add_component_by_id(const EntityId& entity_id,
                                         const ComponentId& component_id,
                                         cereal::JSONInputArchive& in_archive) {
    switch (AllComponentTypes {component_id}) {
        case AllComponentTypes::Body:
            DESERIALIZE_COMPONENT(Body);
            break;
        case AllComponentTypes::Circle:
            DESERIALIZE_COMPONENT(Circle);
            break;
        case AllComponentTypes::AnimationInfo:
            DESERIALIZE_COMPONENT(AnimationInfo);
            break;
        case AllComponentTypes::Timer:
            DESERIALIZE_COMPONENT(Timer);
            break;
        case AllComponentTypes::Render:
            DESERIALIZE_COMPONENT(Render);
            break;
        case AllComponentTypes::Rectangle:
            DESERIALIZE_COMPONENT(Rectangle);
            break;
        case AllComponentTypes::PlayerShape:
            DESERIALIZE_COMPONENT(PlayerShape);
            break;
        case AllComponentTypes::ParallelGravityField:
            DESERIALIZE_COMPONENT(PlayerShape);
            break;
        case AllComponentTypes::CircularGravityField:
            DESERIALIZE_COMPONENT(CircularGravityField);
            break;
        case AllComponentTypes::ColliderObjectFlag:
            DESERIALIZE_COMPONENT(ColliderObjectFlag);
            break;
        case AllComponentTypes::GravFieldFlag:
            DESERIALIZE_COMPONENT(GravFieldFlag);
            break;
        case AllComponentTypes::OutlineRectFlag:
            DESERIALIZE_COMPONENT(OutlineRectFlag);
            break;
        case AllComponentTypes::EntityState:
            DESERIALIZE_COMPONENT(EntityState);
            break;
        case AllComponentTypes::MovedEventData:
            DESERIALIZE_COMPONENT(MovedEventData);
            break;
    }
}
