#pragma once
#include <cstdint>
using EntityId = uint64_t;
constexpr EntityId MAX_ENTITY_IDS = 3000;
const EntityId NO_ENTITY_FOUND = 69420;

// components also have ids
using ComponentId = uint8_t;
constexpr ComponentId MAX_COMPONENTS = 64;

using ComponentMask = uint64_t;
constexpr uint64_t MAX_SYSTEMS = 64;

