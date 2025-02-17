#include "entity_query.hpp"
#include "fumo_engine/component_array.hpp"
// TODO: try again doing this with templates, i failed unfortunately (skill issue)
bool EntityQuery::filter(ComponentMask entity_mask) {
    switch (component_filter) {
    case Filter::All:
        return (entity_mask & component_mask) == component_mask;
    case Filter::Any:
        return (entity_mask & component_mask) != 0;
    case Filter::Only:
        return entity_mask == component_mask;
    case Filter::None:
        return (entity_mask & component_mask) == 0;
    default:
        PANIC("this shouldnt happen");
    }
    return false;
}
