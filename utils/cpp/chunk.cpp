#include "chunk.h"

#include <fstream>
#include <iostream>

#include <fmt/format.h>

namespace fms {
    Chunk::Chunk(const std::uint32_t chunk_width, const Bounds& chunk_bounds)
            : bounds(chunk_bounds)
            , width(chunk_width)
    {
        height_data.reserve(width * width);
    }

    /// Generates all the chunk positions in lon/lat format
    std::vector<Coordinates> Chunk::generate_chunk_coordinates() const {
        const auto [lon_diff, lat_diff] = bounds.get_diffs();
        const auto lon_unit_size = lon_diff / width;
        const auto lat_unit_size = lat_diff / width;

        constexpr double error = 0.01;
        std::vector<Coordinates> coordinates = {};
        coordinates.reserve(width * width);

        // Generate all the chunk positions based on the chunks bounds and width
        double lon = bounds.min.lon;
        while (lon < bounds.max.lon - error) {
            double lat = bounds.min.lat;
            while (lat < bounds.max.lat - error) {
                coordinates.emplace_back(lon, lat);

                lat += lat_unit_size;
            }
            lon += lon_unit_size;
        }

        return coordinates;
    }

    /// Writes the elevation data of the chunk to the designated folder given in the path parameter.
    /// Path: A folder to store the height data
    void Chunk::write_elevation_data(const std::filesystem::path& path, const bool overwrite_data) const {
        const auto filename = std::filesystem::path(static_cast<std::string>(path / bounds.to_string()) + ".dat");

        if (exists(filename) && !overwrite_data) {
            // File is already loaded && we have been told not to overwrite the data if it does exist.
            return;
        }

        // If the file dir does not exist, make it.
        std::string cmd = fmt::format("mkdir -p {0}", filename.parent_path().c_str());
        system(cmd.c_str());

        // Write the file.
        std::ofstream output (filename, std::ios::out | std::ios::binary);
        output.write(reinterpret_cast<const char*>(height_data.data()), sizeof(float) * height_data.size());
        output.close();
    }
}  //namespace fms