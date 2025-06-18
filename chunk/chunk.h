#pragma once

#include "utils/bounds.h"

#include <filesystem>

#include <osmium/area/multipolygon_manager.hpp>

namespace fms {
    namespace types {
        using height_data_type = float;
        using land_data_type = std::int32_t;
    }

    struct GeometryData {
        std::vector<std::string> water;
    };

    struct Chunk {
        Bounds bounds;                             // chunk bounds
        std::uint32_t width;                       // width of the chunk
        std::vector<types::height_data_type> height_data;
        std::vector<types::land_data_type> land_data;
        static constexpr double error =  0.0001;   // Use this value to improve precision when comparing doubles or floats

        explicit Chunk(std::uint32_t chunk_width, const Bounds& chunk_bounds);

        explicit Chunk(const std::filesystem::path& map_file_path, const std::filesystem::path& file_name, std::uint32_t chunk_width);

        [[nodiscard]] std::vector<Coordinates> generate_chunk_coordinates() const;

        void write_elevation_data(const std::filesystem::path& path, bool overwrite_data) const;

        void read_elevation_data(const std::filesystem::path& filename);

        void write_land_data(const std::filesystem::path& path, std::shared_ptr<GeometryData>& geometry_data, bool overwrite_data) const;

        void read_land_data(const std::filesystem::path& filename);

        static Coordinates get_chunk_origin(Coordinates& position, float chunk_width_degrees);
    };
}