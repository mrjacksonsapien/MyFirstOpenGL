#pragma once

#include <cstdint>

enum DirtyFlags : uint32_t {
    DIRTY_NONE       = 0,
    DIRTY_TRANSFORM  = 1 << 0,
    DIRTY_ALL        = 0xFFFFFFFF
};