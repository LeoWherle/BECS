
#pragma once

#include "../utils/debug.hpp"

struct CVelocity {
    float vx, vy;

    DERIVE_DEBUG(CVelocity, vx, vy)
};
