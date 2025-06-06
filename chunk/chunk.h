#pragma once

#include "utils/bounds.h"

#include <filesystem>

namespace fms {
    struct Chunk {
        Bounds bounds;                             // chunk bounds
        std::uint32_t width;                       // width of the chunk
        std::vector<float> height_data;
        static constexpr double error =  0.0001;   // Use this value to improve precision when comparing doubles or floats

        explicit Chunk(std::uint32_t chunk_width, const Bounds& chunk_bounds);

        explicit Chunk(const std::filesystem::path& file_path);

        [[nodiscard]] std::vector<Coordinates> generate_chunk_coordinates() const;

        void write_elevation_data(const std::filesystem::path& path, bool overwrite_data) const;

        void read_elevation_data(const std::filesystem::path& path);

        static Coordinates get_chunk_origin(Coordinates& position, float chunk_width_degrees);
    };
}