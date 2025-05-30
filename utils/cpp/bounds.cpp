#include "bounds.h"

#include <fmt/format.h>

namespace fms {
    Bounds::Bounds(const std::vector<Coordinates>& coords_vec) {
        // Finds the min and max coordinates from the input list and assigns them to their respective bounds.
        for (const auto& coords : coords_vec) {
            update_bounds(coords);
        }
    }

    /// Updates the bounds if the given coordinates are less than the current min or greater than the current max
    void Bounds::update_bounds(const Coordinates& coords) {
        const auto [lon, lat] = coords;
        min.lon = std::min(min.lon, lon);
        min.lat = std::min(min.lat, lat);
        max.lon = std::max(max.lon, lon);
        max.lat = std::max(max.lat, lat);
    }

    /// Returns the max - min of each the latitude and the longitude in the form of a Coordinate
    Coordinates Bounds::get_diffs() const {
        return {max.lon - min.lon, max.lat - min.lat};
    }

    /// Converts the bounds to a string format
    /// This is useful when writing to files or using as a key
    std::string Bounds::to_string() const {
        return fmt::format("{:.2f}_{:.2f}_{:.2f}_{:.2f}", min.lon, min.lat, max.lon, max.lat);
    }
}  // namespace fms