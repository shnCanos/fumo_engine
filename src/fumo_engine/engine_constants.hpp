#ifndef CONSTANTS_USING_HPP
#define CONSTANTS_USING_HPP
#include <cstdint>
using EntityId = uint64_t;
const EntityId MAX_ENTITY_IDS = 100;
const EntityId NO_ENTITY_FOUND = 69420;
// components also have ids
using ComponentId = uint8_t;
const ComponentId MAX_COMPONENTS = 64;

using ComponentMask = uint64_t;
const uint64_t MAX_SYSTEMS = 64;
#endif
