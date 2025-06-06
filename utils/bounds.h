#pragma once

#include "coordinates.h"

#include <string>
#include <vector>

namespace fms {
    struct Bounds {
        mutable Coordinates min = { 180,  90};
        mutable Coordinates max = {-180, -90};

        Bounds() = default;

        explicit Bounds(const std::vector<Coordinates>& coords_vec);

        explicit Bounds(const std::string& bounds_string);

        void update_bounds(const Coordinates& coords);

        void bounds_from_string(const std::string& bounds_string);

        bool operator>=(const Bounds& other) const;

        [[nodiscard]] Coordinates get_diffs() const;

        [[nodiscard]] std::string to_string() const;

        void print();
    };
}