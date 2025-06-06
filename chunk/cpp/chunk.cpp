#include "chunk.h"

#include <fstream>
#include <iostream>

#include <fmt/format.h>

/*
 *  Chunk Rules:
 *      1. The size of the units within a chunk will always be width * width (meaning it is a square)
 *          - Therefore, a chunks width can be determined by obtaining the square root of the number of units in a chunk
 */

namespace fms {
    Chunk::Chunk(const std::uint32_t chunk_width, const Bounds& chunk_bounds)
            : bounds(chunk_bounds)
            , width(chunk_width)
    {
        height_data.reserve(width * width);
    }

    /// Loads the chunk data directly from a file
    Chunk::Chunk(const std::filesystem::path& file_path) {
        const std::string filename = file_path.stem().c_str();

        read_elevation_data(file_path);

        width = static_cast<std::uint32_t>(std::sqrt(height_data.size()));
        bounds = Bounds(filename);
    }

    /// Generates all the chunk positions in lon/lat format
    std::vector<Coordinates> Chunk::generate_chunk_coordinates() const {
        const auto [lon_diff, lat_diff] = bounds.get_diffs();
        const auto lon_unit_size = lon_diff / width;
        const auto lat_unit_size = lat_diff / width;

        std::vector<Coordinates> coordinates = {};
        coordinates.reserve(width * width);

        // Generate all the chunk positions based on the chunks bounds and width
        double lat = bounds.min.lat;
        while (lat < bounds.max.lat - error) {
            double lon = bounds.min.lon;
            while (lon < bounds.max.lon - error) {
                coordinates.emplace_back(lon, lat);
                lon += lon_unit_size;
            }
            lat += lat_unit_size;
        }

        return coordinates;
    }

    /// Writes the elevation data of the chunk to the designated folder given in the path parameter.
    /// Path: A folder to store the height data
    void Chunk::write_elevation_data(const std::filesystem::path& path, const bool overwrite_data) const {
        const auto filename = std::filesystem::path(static_cast<std::string>(path / bounds.to_string()) + ".dat");

        if (exists(filename) && !overwrite_data) {
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

    /// Reads elevation data from a specified file path and stores it in the .height_data member
    void Chunk::read_elevation_data(const std::filesystem::path& path) {
        float f;
        std::ifstream fin (path, std::ifstream::binary);
        while (fin.read(reinterpret_cast<char*>(&f), sizeof(float))) {
            height_data.emplace_back(f);
        }
    }

    /// Calculate the bottom left coordinates of the current chunk
    Coordinates Chunk::get_chunk_origin(Coordinates& position, const float chunk_width_degrees) {
        // Negative values must be altered to calculate this function correctly.
        if (position.lon < 0) {
            position.lon -= chunk_width_degrees;
        }
        if (position.lat < 0) {
            position.lat -= chunk_width_degrees;
        }

        const auto lon_remainder = std::fmod(position.lon, chunk_width_degrees);
        const auto lat_remainder = std::fmod(position.lat, chunk_width_degrees);

        return {
            position.lon - lon_remainder,
            position.lat - lat_remainder
        };
    }
}  //namespace fms