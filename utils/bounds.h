#pragma once

#include <fmt/format.h>

#include <string>
#include <vector>

namespace fms {
    using longitude = double;
    using latitude  = double;

    struct Coordinates {
        double lon;
        double lat;

        Coordinates(const longitude lon_in, const latitude lat_in)
            : lon(lon_in)
            , lat(lat_in)
        { }

        /// Expects the Coordinate pair to be in the format of {longitude, latitude}
        explicit Coordinates(const std::pair<longitude, latitude>& coords)
            : lon(coords.first)
            , lat(coords.second)
        { }
    };

    struct Bounds {
        Coordinates min = { 180,  90};  // Set to the maximum lat/lon by default so they are always replaced with a < comparison
        Coordinates max = {-180, -90};  // Set to the minimum lat/lon by default so they are always replaced with a > comparison

        explicit Bounds(const std::vector<Coordinates>& coords_vec) {
            for (const auto& coords : coords_vec) {
                update_bounds(coords);
            }
        }

        void update_bounds(const Coordinates& coords) {
            const auto [lon, lat] = coords;
            min.lon = std::min(min.lon, lon);
            min.lat = std::min(min.lat, lat);
            max.lon = std::max(max.lon, lon);
            max.lat = std::max(max.lat, lat);
        }

        [[nodiscard]] Coordinates get_diffs() const {
            return {max.lon - min.lon, max.lat - min.lat};
        }

        [[nodiscard]] std::string to_string() const {
            return fmt::format("{:.2f}_{:.2f}_{:.2f}_{:.2f}", min.lon, min.lat, max.lon, max.lat);
        }
    };
}