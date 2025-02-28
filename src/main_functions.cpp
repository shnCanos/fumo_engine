#include "main_functions.hpp"
Vector2 Vector2ScaleDivision(Vector2 v, float scale) {
    Vector2 result = {v.x / scale, v.y / scale};
    return result;
}

void debug_print_animation_info(const AnimationInfo& animation_info) {
    PRINT(animation_info.current_region_rect.x);
    PRINT(animation_info.current_region_rect.y);
    // PRINT(animation_info.current_region_rect.width);
    // PRINT(animation_info.current_region_rect.height);
    PRINT(animation_info.current_sheet_name);
    PRINT(animation_info.frame_progress);
    std::cerr << '\n';
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
