#include "fumo_engine/components.hpp"

void FumoRect::draw_outline(const FumoColor& color,
                            const FumoVec2& position) const {
    FumoVec2 TopLeft = {position.x, position.y};
    FumoVec2 TopRight = {position.x + width, position.y};
    FumoVec2 BottomLeft = {position.x, position.y + height};
    FumoVec2 BottomRight = {position.x + width, position.y + height};
    FumoDrawLineV(TopLeft, TopRight, color);
    FumoDrawLineV(TopLeft, BottomLeft, color);
    FumoDrawLineV(TopRight, BottomRight, color);
    FumoDrawLineV(BottomLeft, BottomRight, color);
}

void FumoRect::draw(const FumoColor& color, const FumoVec2& position) const {
    FumoDrawRectV(position, {width, height}, color);
}

void Circle ::draw(const FumoColor& color, const FumoVec2& position) const {
    FumoDrawCircleV(position, radius, color);
}

void CircularGravityField::draw(const FumoColor& color,
                                const FumoVec2& position) const {
    FumoDrawCircleV(position, position.y, FumoColorAlpha(color, 0.2f));
}

void ParallelGravityField::draw(const FumoColor& color,
                                const FumoVec2& position) const {
    FumoDrawRectV(position,
                  {field_fumo_rect.width, field_fumo_rect.height},
                  FumoColorAlpha(color, 0.2f));
}
