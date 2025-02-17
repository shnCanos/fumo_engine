#include "main_functions.hpp"
Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
    Vector2 result = {v.x / scale, v.y / scale};
    return result;
}
// template<>
// bool FilterComponents<Filter::All>::filter(ComponentMask component_mask,
//                                                     ComponentMask target_components) {
//     return (component_mask & target_components) == target_components;
// }
// template<>
// bool FilterComponents<Filter::Any>::filter(ComponentMask component_mask,
//                                                     ComponentMask target_components) {
//     return (component_mask & target_components) != 0;
// }
// template<>
// bool FilterComponents<Filter::Only>::filter(ComponentMask component_mask,
//                                                      ComponentMask target_components) {
//     return component_mask == target_components;
// }
// template<>
// bool FilterComponents<Filter::None>::filter(ComponentMask component_mask,
//                                                      ComponentMask target_components) {
//
//     return (component_mask & target_components) == 0;
// }
