#include "bounds.h"

#include <regex>

#include <fmt/format.h>
#include <iostream>

namespace fms {
    Bounds::Bounds(double min_lon, double min_lat, double max_lon, double max_lat)
        : min(min_lon, min_lat)
        , max(max_lon, max_lat)
    { }

    Bounds::Bounds(const std::vector<Coordinates>& coords_vec) {
        // Finds the min and max coordinates from the input list and assigns them to their respective bounds.
        for (const auto& coords : coords_vec) {
            update_bounds(coords);
        }
    }

    Bounds::Bounds(const std::string& bounds_string, const types::parse string_type) {
        switch(string_type) {
        case types::parse::FILENAME:
            bounds_from_string(bounds_string);
        case types::parse::WKT:
            bounds_from_WKT(bounds_string);
        default:
            bounds_from_string(bounds_string);
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

    void Bounds::bounds_from_WKT(const std::string& WKT) {
        std::vector<Coordinates> coords_vec;

        std::smatch base_match;
        std::regex pattern(R"(-?\d+\.?\d*)");
        std::sregex_iterator iter(WKT.begin(), WKT.end(), pattern);
        std::sregex_iterator end;

        for (; iter != end; ++iter) {
            coords_vec.push_back({
                std::stod(iter->str()),
                std::stod(std::next(iter)->str())
            });
            ++iter;
        }

        for (const auto& coords : coords_vec) {
            update_bounds(coords);
        }
    }

    // Returns a bounds which is the intersection between this bounds and the bounds passed in as "other"
    // If there is no intersection between the bounds, the diff of the resulting bounds will identical to this bounds.
    Bounds Bounds::intersection(const Bounds& other) const {
        return {
            std::clamp(other.min.lon, min.lon, max.lon),
            std::clamp(other.min.lat, min.lat, max.lat),
            std::clamp(other.max.lon, min.lon, max.lon),
            std::clamp(other.max.lat, min.lat, max.lat)
        };
    }

    Bounds Bounds::operator-(const Bounds& other) const {
        return {
            min.lon - other.min.lon,
            min.lat - other.min.lat,
            max.lon - other.max.lon,
            max.lat - other.max.lat
        };
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