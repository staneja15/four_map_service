#pragma once

#include <vector>

using longitude = double;
using latitude  = double;

namespace fms {
    struct Coordinates {
        double lon;
        double lat;

        Coordinates(longitude lon_in, latitude lat_in);

        /// Expects the Coordinate pair to be in the format of {longitude, latitude}
        explicit Coordinates(const std::pair<longitude, latitude>& coords);
    };
}  // namespace fms