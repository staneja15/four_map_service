#include "bounds.h"

#include <regex>

#include <fmt/format.h>
#include <iostream>

namespace fms {
    Bounds::Bounds(const std::vector<Coordinates>& coords_vec) {
        // Finds the min and max coordinates from the input list and assigns them to their respective bounds.
        for (const auto& coords : coords_vec) {
            update_bounds(coords);
        }
    }

    Bounds::Bounds(const std::string& bounds_string) {
        bounds_from_string(bounds_string);
    }

    /// Updates the bounds if the given coordinates are less than the current min or greater than the current max
    void Bounds::update_bounds(const Coordinates& coords) {
        const auto [lon, lat] = coords;
        min.lon = std::min(min.lon, lon);
        min.lat = std::min(min.lat, lat);
        max.lon = std::max(max.lon, lon);
        max.lat = std::max(max.lat, lat);
    }

    void Bounds::bounds_from_string(const std::string& bounds_string) {
        std::smatch base_match;
        const std::regex pattern("(-?[0-9]+.[0-9]+)_(-?[0-9]+.[0-9]+)_(-?[0-9]+.[0-9]+)_(-?[0-9]+.[0-9]+)");
        std::regex_match(bounds_string, base_match, pattern);

        // base_match often has empty strings, making the size larger than the number of matches
        // base_match[0] is the entire string, and subsequent populated strings are the matches
        // we expect 4 matches, one for each bound
        if (base_match.size() >= 5) {
            min.lon = std::stod(base_match[1]);
            min.lat = std::stod(base_match[2]);
            max.lon = std::stod(base_match[3]);
            max.lat = std::stod(base_match[4]);
        }
    }

    /// Returns true if all values of other bounds is within or equal to this bounds
    bool Bounds::operator>=(const Bounds& other) const {
        return min.lon <= other.min.lon
            && min.lat <= other.min.lat
            && max.lon >= other.max.lon
            && max.lat >= other.max.lat;
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

    void Bounds::print() {
        std::cout << to_string() << "\n";
    }
}  // namespace fms