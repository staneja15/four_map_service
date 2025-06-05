#pragma once

#include "utils/bounds.h"

#include <cstdint>

namespace fms {
    struct MapGenerationInfo {
        Bounds bounds;
        float chunk_width_degrees;   // Degrees is the unit of measurement used for latitude and longitude data
        std::uint32_t width;         // The number of units along an axis of the chunk (width * width = number of units in a chunk)
        bool padding;                // Whether to add a unit of padding along the top and right edges of the bounds. This is useful when rendering a grid.
    };
}