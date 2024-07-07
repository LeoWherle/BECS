
#pragma once

#include "raylib.h"
#include "systems.hpp"

class SRenderCircle {
public:
    void render(World &world)
    {
        auto view = world.getView<CPosition, CCircle, CShapeColor>();
        view.each([](Entity entity, CPosition &pos, CCircle &size, CShapeColor &color) {
            DrawCircle(pos.x, pos.y, size.radius, Color {color.r, color.g, color.b, color.a});
        });
    }
};
