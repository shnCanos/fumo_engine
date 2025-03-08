#ifndef ENTITY_QUERY_HPP
#define ENTITY_QUERY_HPP
#include "engine_constants.hpp"
// offer template specializations for the 4 kinds of component filters we can pass in
// and create this sytems's entity query class which offers the system a query() method
// or a get_entities() method

/*
    All = All component ids in this array must exist in
    Any = At least one of the component ids in this array must exist
    Only = Only the component ids in this array must exist
    None = None of the component ids in this array can exist
*/
/*
   TODO:
   - Shared component values — filter the set of entities
     based on specific values of a shared component.
   - Change filter — Filter the set of entities based on whether the value
     of a specific component type has potentially changed.
*/
// TODO: review our EntityQuery implementation (might need redoing)
// moving on for now, when we use this more i will rethink it

enum class Filter { All, Any, Only, None };
class EntityQuery {
public:
    ComponentMask component_mask;
    Filter component_filter;
    bool filter(ComponentMask entity_mask);
};
#endif
