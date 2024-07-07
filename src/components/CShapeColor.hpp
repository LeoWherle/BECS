
#pragma once

#include "../utils/debug.hpp"
#include <cstdint>

struct CShapeColor {
    union {
        struct {
            uint8_t r, g, b, a;
        };
        uint32_t value;
    };

    DERIVE_DEBUG(CShapeColor, r, g, b, a)
};
