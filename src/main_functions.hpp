
#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP
#include "objects/components.hpp"
#include "raylib.h"
Vector2 Vector2ScaleDivision(Vector2 v, float scale);
// bool has_all_components(ComponentMask component_mask, ComponentMask target_components);
// bool only_has_components(ComponentMask component_mask, ComponentMask target_components);
// bool has_no_components(ComponentMask component_mask, ComponentMask target_components);
// bool has_any_component(ComponentMask component_mask, ComponentMask target_components);
void debug_print_animation_info(const AnimationInfo& animation_info);

// template<ComponentFilter>
// class FilterComponents {
// public:
//     bool filter(ComponentMask component_mask, ComponentMask target_components) {}
// };

#endif
