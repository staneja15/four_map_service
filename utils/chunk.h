#pragma once

#include "bounds.h"

#include <filesystem>

namespace fms {
    struct Chunk {
        Bounds bounds;                             // chunk bounds
        std::uint32_t width;                       // width of the chunk
        std::vector<float> height_data;

        explicit Chunk(std::uint32_t chunk_width, const Bounds& chunk_bounds);

        [[nodiscard]] std::vector<Coordinates> generate_chunk_coordinates() const;

        void write_elevation_data(const std::filesystem::path& path, const bool overwrite_data) const;
    };
}