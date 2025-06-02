#include "chunk_manager.h"

namespace fms {
    // map_bounds is the bounds of the map file you wish to load with the ChunkManager
    //   This should reflect the folder name in the data/ directory
    ChunkManager::ChunkManager(const std::filesystem::path& data_directory)
        : _data_path(data_directory)
    { }

    void ChunkManager::load_chunk_data() {
        _chunks = {};

        // todo: add an option to only load chunks within a set bounds. This will speed this function up.
        for (const auto& map_directory : std::filesystem::directory_iterator(_data_path)) {
            for (const auto& file : std::filesystem::directory_iterator(_data_path / map_directory.path().c_str())) {
                _chunks.emplace_back(std::make_shared<Chunk>(file));
            }
        }
    }

    std::vector<std::shared_ptr<Chunk>> ChunkManager::get_chunks() const {
        return _chunks;
    }
}  // namespace fms