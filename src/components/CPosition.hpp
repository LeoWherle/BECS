
#pragma once

#include "../utils/debug.hpp"

struct CPosition {
    float x, y;

    DERIVE_DEBUG(CPosition, x, y)
};
