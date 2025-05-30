#include "coordinates.h"

namespace fms {
    Coordinates::Coordinates(longitude lon_in, latitude lat_in)
        : lon(lon_in)
        , lat(lat_in)
    { }

    Coordinates::Coordinates(const std::pair<longitude, latitude>& coords)
        : lon(coords.first)
        , lat(coords.second)
    { }
}  // namespace fms