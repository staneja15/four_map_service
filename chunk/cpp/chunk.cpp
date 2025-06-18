#include "chunk.h"
#include "file_system/file_system.h"

#include <fstream>
#include <execution>

#include <fmt/format.h>
#include <gdal/ogr_geometry.h>


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
        land_data.reserve(width * width);
    }

    /// Loads the chunk data directly from a file
    Chunk::Chunk(const std::filesystem::path& map_file_path, const std::filesystem::path& file_name)
        : height_data({})
        , land_data({})
    {
        const std::filesystem::path height_dir = map_file_path / "height";
        const std::filesystem::path land_dir = map_file_path / "land";
        const std::string file_name_no_ext = file_name.stem().c_str();

        read_elevation_data(height_dir / file_name);
        read_land_data(land_dir / file_name);

        width = static_cast<std::uint32_t>(std::sqrt(height_data.size()));
        bounds = Bounds(file_name_no_ext, types::parse::FILENAME);
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

        write_to_binary(filename, height_data);
    }

    /// Reads elevation data from a specified file path and stores it in the .height_data member
    void Chunk::read_elevation_data(const std::filesystem::path& filename) {
        read_binary(filename, height_data);
    }

    void Chunk::write_land_data(const std::filesystem::path& path, std::shared_ptr<GeometryData>& geometry_data, bool overwrite_data) const {
        const auto filename = std::filesystem::path(static_cast<std::string>(path / bounds.to_string()) + ".dat");

        if (exists(filename) && !overwrite_data) {
            return;
        }

        std::vector<Coordinates> coords = generate_chunk_coordinates();
        std::vector<types::land_data_type> type_mask;
        type_mask.resize(width * width);

        OGRLinearRing ogr_bounds;
        ogr_bounds.addPoint(bounds.min.lon, bounds.min.lat);  // Bottom left
        ogr_bounds.addPoint(bounds.min.lon, bounds.max.lat);  // Top left
        ogr_bounds.addPoint(bounds.max.lon, bounds.max.lat);  // Top right
        ogr_bounds.addPoint(bounds.max.lon, bounds.min.lat);  // Bottom right
        ogr_bounds.addPoint(bounds.min.lon, bounds.min.lat);  // Close ring

        OGRPolygon box;
        box.addRing(&ogr_bounds);

        // Add water to the type mask
        for (auto& WKT : geometry_data->water) {
            OGRGeometry* multi_polygon = nullptr;
            OGRGeometryFactory::createFromWkt(WKT.c_str(), nullptr, &multi_polygon);

            const OGRBoolean disjoint = multi_polygon->Disjoint(&box);

            if (disjoint) {
                // Early exit if the polygon does not overlap with the chunk boundaries
                continue;
            }

            auto poly_bounds = Bounds(WKT, types::parse::WKT);
            auto intersection = bounds.intersection(poly_bounds);

            const auto [lon_diff, lat_diff] = bounds.get_diffs();
            const auto lon_unit_size = lon_diff / width;
            const auto lat_unit_size = lat_diff / width;

            auto diff = intersection - bounds;

            std::size_t lat_idx = diff.min.lat / lat_unit_size;
            while (lat_idx < width) {
                std::size_t lon_idx = diff.min.lon / lon_unit_size;
                while (lon_idx < width) {
                    const std::size_t index = lat_idx * width + lon_idx;
                    OGRPoint point(coords[index].lon, coords[index].lat);
                    if (multi_polygon->Contains(&point)) {
                        type_mask[index] = 1;
                    }
                    ++lon_idx;
                }
                ++lat_idx;
            }
        }

        write_to_binary(filename, type_mask);
    }

    void Chunk::read_land_data(const std::filesystem::path& filename) {
        read_binary(filename, land_data);
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