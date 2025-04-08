#include <fstream>

#include "filesystem"
#include "fumo_engine/core/global_state.hpp"
#include "fumo_engine/level_systems/level_manager.hpp"

extern std::unique_ptr<GlobalState> global;
// NOTE: maybe switch this to a namespace later if this class is useless

namespace fs = std::filesystem;

void LevelSerializer::serialize_levels() {
    for (const auto& entity_id : sys_entities) {
        const auto& screen_id = global->ECS->get_component<ScreenId>(entity_id);
        const auto& level_id = global->ECS->get_component<LevelId>(entity_id);

        fs::create_directory(std::format("level{}", level_id.level_id));

        std::ofstream out_stream(std::format("screen{}.json", screen_id.screen_id));

        if (out_stream.fail()) [[unlikely]]
            PANIC("failed to open or create file\n");

        cereal::JSONOutputArchive out_archive(out_stream);

    }
};

