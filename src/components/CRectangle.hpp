
#pragma once

#include "../utils/debug.hpp"


struct CRectangle {
    float width, height;

    DERIVE_DEBUG(CRectangle, width, height)
};
