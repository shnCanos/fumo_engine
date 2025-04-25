
#pragma once

namespace Initialization {
void register_all_to_fumo_engine();
void register_components();
void register_systems();
void register_unregistered_systems_unscheduled();
void register_systems_scheduled();

void load_player_textures();
void initialize_all_textures();

void initialize_window();
} // namespace Initialization
