#include "chunk_manager.h"

#include <fmt/format.h>

namespace fms {
    // map_bounds is the bounds of the map file you wish to load with the ChunkManager
    //   This should reflect the folder name in the data/ directory
    ChunkManager::ChunkManager(const std::filesystem::path& data_directory)
        : _data_path(data_directory)
        , _chunk_origin(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN())
    { }

    bool ChunkManager::update_position(Coordinates& position, float chunk_width_degrees, std::uint32_t chunk_width_n) {
        const auto new_origin = Chunk::get_chunk_origin(position, chunk_width_degrees);

        if (!check_chunk_has_changed(new_origin)) {
            return false;
        }

        _chunk_origin = new_origin;
        _load_chunk_data(chunk_width_degrees, chunk_width_n);

        return true;
    }

    bool ChunkManager::check_chunk_has_changed(const Coordinates& chunk_origin) const {
        return _chunk_origin != chunk_origin;
    }

    std::vector<std::shared_ptr<Chunk>> ChunkManager::get_chunks() const {
        return _chunks;
    }

    void ChunkManager::_load_chunk_data(const float chunk_width_degrees, const std::uint32_t chunk_width_n) {
        _chunks = {};

        // Define the bounds of the chunk manager based on the input parameters
        const auto chunk_manager_bounds = Bounds(
            {
                Coordinates {
                    _chunk_origin.lon - (static_cast<double>(chunk_width_n) * chunk_width_degrees),
                    _chunk_origin.lat - (static_cast<double>(chunk_width_n) * chunk_width_degrees)
                },
                Coordinates {
                    _chunk_origin.lon + (static_cast<double>(chunk_width_n) * chunk_width_degrees),
                    _chunk_origin.lat + (static_cast<double>(chunk_width_n) * chunk_width_degrees)
                },
            }
        );

        chunk_manager_bounds.max = chunk_manager_bounds.max + chunk_width_degrees + Chunk::error;
        chunk_manager_bounds.min = chunk_manager_bounds.min - Chunk::error;

        // Load chunks from file if they are within the chunk manager bounds
        for (const auto& map_directory : std::filesystem::directory_iterator(_data_path)) {
            for (const auto& file : std::filesystem::directory_iterator(_data_path / map_directory.path().c_str())) {
                const std::string filename = file.path().stem().c_str();
                auto bounds = Bounds {filename};
                if (chunk_manager_bounds >= bounds) {
                    _chunks.emplace_back(std::make_shared<Chunk>(file));
                }
            }
        }
    }
}  // namespace fms