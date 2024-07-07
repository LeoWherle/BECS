
#pragma once

#include "systems.hpp"

class SCollision {
private:
    float minX, minY, maxX, maxY;

public:
    SCollision(float minX, float minY, float maxX, float maxY):
        minX(minX),
        minY(minY),
        maxX(maxX),
        maxY(maxY)

    {
    }

    void update(World &world)
    {
        auto view = world.getView<CPosition, CVelocity, CCircle>();
        view.each([this](Entity entity, CPosition &pos, CVelocity &vel, CCircle &size) {
            if (pos.x - size.radius <= minX || pos.x + size.radius >= maxX) {
                vel.vx = -vel.vx;
                pos.x = std::clamp(pos.x, minX + size.radius, maxX - size.radius);
            }
            if (pos.y - size.radius <= minY || pos.y + size.radius >= maxY) {
                vel.vy = -vel.vy;
                pos.y = std::clamp(pos.y, minY + size.radius, maxY - size.radius);
            }
        });
        auto view2 = world.getView<CPosition, CVelocity, CRectangle>();
        view2.each([this](Entity entity, CPosition &pos, CVelocity &vel, CRectangle &size) {
            if (pos.x <= minX || pos.x + size.width >= maxX) {
                vel.vx = -vel.vx;
                pos.x = std::clamp(pos.x, minX, maxX - size.width);
            }
            if (pos.y <= minY || pos.y + size.height >= maxY) {
                vel.vy = -vel.vy;
                pos.y = std::clamp(pos.y, minY, maxY - size.height);
            }
        });
    }
};