#pragma once

#include "coordinates.h"

#include <string>
#include <vector>

namespace fms {
    namespace types {
        enum class parse {
            FILENAME,
            WKT
        };
    }

    struct Bounds {
        mutable Coordinates min = { 180,  90};
        mutable Coordinates max = {-180, -90};

        Bounds() = default;

        Bounds(double min_lon, double min_lat, double max_lon, double max_lat);

        explicit Bounds(const std::vector<Coordinates>& coords_vec);

        explicit Bounds(const std::string& bounds_string, types::parse string_type);

        void update_bounds(const Coordinates& coords);

        void bounds_from_string(const std::string& bounds_string);

        void bounds_from_WKT(const std::string& WKT);

        Bounds intersection(const Bounds& other) const;

        Bounds operator-(const Bounds& other) const;

        bool operator>=(const Bounds& other) const;

        [[nodiscard]] Coordinates get_diffs() const;

        [[nodiscard]] std::string to_string() const;

        void print();
    };
}