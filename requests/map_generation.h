#pragma once

#include "utils/bounds.h"

#include <cstdint>

namespace fms {
    struct MapGenerationInfo {
        Bounds bounds;
        double chunk_width_degrees;  // Degrees is the unit of measurement used for latitude and longitude data
        std::uint32_t width;         // The number of units along an axis of the chunk (width * width = number of units in a chunk)
    };
}