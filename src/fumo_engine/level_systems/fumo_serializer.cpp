#include "fumo_engine/level_systems/fumo_serializer.hpp"

#include <fstream>

#include "filesystem"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/level_systems/fumo_serializer.hpp"
#include "fumo_engine/level_systems/level_editor.hpp"

extern std::unique_ptr<GlobalState> global;

namespace fs = std::filesystem;

void clear_files_for_serialization() {

    fs::copy(fs::current_path(),
             "../backup_serialized_data",
             fs::copy_options::recursive | fs::copy_options::overwrite_existing);

    for (const auto& directory : fs::directory_iterator(fs::current_path())) {

        if (!directory.is_directory()) [[unlikely]] {
            PANIC("added a file into serialized_data (should only have directories)");
        }
        fs::remove_all(directory);
    }
}

void fix_JSON_serialization() {
    for (const auto& directory : fs::directory_iterator(fs::current_path())) {

        if (!directory.is_directory()) [[unlikely]] {
            PANIC("added a file into serialized_data (should only have directories)");
        }

        for (const auto& dir_entry : fs::directory_iterator(directory)) {
            const auto file_name = dir_entry.path();

            std::ifstream in_stream(file_name);
            std::stringstream cereal_buffer;

            if (in_stream.fail()) [[unlikely]]
                PANIC("failed to open file for ifstream.\n");

            cereal_buffer << in_stream.rdbuf();
            std::string contents = cereal_buffer.str();

            contents.insert(0, "{\n");
            contents.append("}");

            std::ofstream out_stream(file_name, std::ios::trunc);
            out_stream << contents;
        }
    }
}

void LevelSerializer::serialize_levels() {
    // NOTE:  assumes that we have changed the directory to serialized_data/

    // TODO: add data versioning to the serialized_data

    clear_files_for_serialization();

    for (const auto& entity_id : sys_entities) {
        const auto& screen_id = global->ECS->get_component<ScreenId>(entity_id);
        const auto& level_id = global->ECS->get_component<LevelId>(entity_id);

        fs::path level_screen_path = std::format("level{}", level_id.level_id);
        fs::create_directory(level_screen_path);

        level_screen_path /= std::format("screen{}.json", screen_id.screen_id);

        std::ofstream out_stream(level_screen_path, std::ios::app);

        std::stringstream cereal_buffer;

        if (out_stream.fail()) [[unlikely]]
            PANIC("failed to open or create file\n");

        {
            // guarantee that cereal flushes out its buffer
            cereal::JSONOutputArchive out_archive(cereal_buffer);
            FumoSerializer::serialize_entity(entity_id, out_archive);
        }

        std::string contents = cereal_buffer.str();
        contents = contents.substr(2, contents.length() - 4);
        contents.append(",\n");

        out_stream << contents;
    }
    // edit the file so it matches the correct JSON format
    // after writing to all files, go back and append { and } to the files
    fix_JSON_serialization();
};

// structure for the json is:
// entity id
// component_mask
// all components
namespace FumoSerializer {

#define DESERIALIZE_COMPONENT(Type) \
    global->ECS->entity_add_component( \
        entity_id, \
        FumoSerializer::deserialize_component<Type>(#Type, \
                                                    component_id, \
                                                    in_archive))

void deserialize_component_by_id(const EntityId& entity_id,
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
        case AllComponentTypes::FumoRect:
            DESERIALIZE_COMPONENT(FumoRect);
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
        case AllComponentTypes::ScreenId:
            DESERIALIZE_COMPONENT(ScreenId);
            break;
        case AllComponentTypes::LevelId:
            DESERIALIZE_COMPONENT(LevelId);
            break;
    }
}
} // namespace FumoSerializer

namespace FumoSerializer {

void serialize_entity(const EntityId& entity_id,
                      cereal::JSONOutputArchive& out_archive) {
    auto component_mask = global->ECS->get_component_mask(entity_id);

    out_archive(CEREAL_NVP(entity_id));
    out_archive(CEREAL_NVP(component_mask));

    // global->ECS->debug_print_entity(entity_id);

    for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
        uint64_t iterate = 1;
        iterate <<= id;
        if (component_mask & (iterate)) {
            global->ECS->serialize_component(entity_id, id, out_archive);
        }
    }
}

void deserialize_entity(cereal::JSONInputArchive& in_archive) {
    // follows the save order for serialized data
    EntityId entity_id;
    ComponentMask component_mask;
    // std::cerr << "component_mask: " << std::format("{:064b}", component_mask) << " | " << std::endl;
    in_archive(entity_id, component_mask);

    for (ComponentId id = 0; id < MAX_COMPONENTS; ++id) {
        uint64_t iterate = 1;
        iterate <<= id;
        if (component_mask & (iterate)) {
            FumoSerializer::deserialize_component_by_id(entity_id, id, in_archive);
        }
    }
}

} // namespace FumoSerializer

void LevelSerializer::deserialize_levels() {
    // assumes we are in the serialized_data/ directory

    for (const auto& directory : fs::directory_iterator(fs::current_path())) {

        if (!directory.is_directory()) [[unlikely]] {
            PANIC("added a file into serialized_data (should only have directories)");
        }

        for (const auto& dir_entry : fs::directory_iterator(directory)) {
            const auto file_name = dir_entry.path();

            std::ifstream in_stream(file_name);

            if (in_stream.fail()) [[unlikely]]
                PANIC("failed to open file for ifstream.\n");

            cereal::JSONInputArchive in_archive(in_stream);

            FumoSerializer::deserialize_entity(in_archive);
        }
    }
}
