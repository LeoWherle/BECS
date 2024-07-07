
#pragma once

#include "systems.hpp"

class SMovement {
public:
    void update(World &world, float deltaTime)
    {
        auto view = world.getView<CPosition, CVelocity>();
        view.each([deltaTime](Entity entity, CPosition &pos, CVelocity &vel) {
            pos.x += vel.vx * deltaTime;
            pos.y += vel.vy * deltaTime;
        });
    }
};
