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

    bool Coordinates::operator==(const Coordinates& other) const {
        return lon == other.lon && lat == other.lat;
    }

    bool Coordinates::operator!=(const Coordinates& other) const {
        return lat != other.lat || lon != other.lon;
    }

    Coordinates Coordinates::operator-(const Coordinates& other) const {
        return {
            lon - other.lon,
            lat - other.lat
        };
    }

    Coordinates Coordinates::operator-(const double other) const {
        return {
            lon - other,
            lat - other
        };
    }

    Coordinates Coordinates::operator+(const Coordinates& other) const {
        return {
            lon + other.lon,
            lat + other.lat
        };
    }

    Coordinates Coordinates::operator+(const double other) const {
        return {
            lon + other,
            lat + other
        };
    }
}  // namespace fms