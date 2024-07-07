
#pragma once

#include "../World.hpp"
#include "../components/components.hpp"

#include "SCollision.hpp"
#include "SMovement.hpp"
#include "SRenderCircle.hpp"
#include "raylib.h"

class SRenderRectangle {
public:
    void render(World &world)
    {
        auto view = world.getView<CPosition, CRectangle, CShapeColor>();
        view.each([](Entity entity, CPosition &pos, CRectangle &size, CShapeColor &color) {
            DrawRectangle(pos.x, pos.y, size.width, size.height, Color {color.r, color.g, color.b, color.a});
        });
    }
};
