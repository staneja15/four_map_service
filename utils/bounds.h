#pragma once

#include "coordinates.h"

#include <string>
#include <vector>

namespace fms {
    struct Bounds {
        Coordinates min = { 180,  90};  // Set to the maximum lat/lon by default so they are always replaced with a < comparison
        Coordinates max = {-180, -90};  // Set to the minimum lat/lon by default so they are always replaced with a > comparison

        explicit Bounds(const std::vector<Coordinates>& coords_vec);

        void update_bounds(const Coordinates& coords);

        [[nodiscard]] Coordinates get_diffs() const;

        [[nodiscard]] std::string to_string() const;
    };
}