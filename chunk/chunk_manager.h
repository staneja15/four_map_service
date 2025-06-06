#pragma once

#include "chunk.h"

#include "utils/bounds.h"

namespace fms {
    class ChunkManager {
    public:
        explicit ChunkManager(const std::filesystem::path& data_directory);

        bool update_position(Coordinates& position, float chunk_width_degrees, std::uint32_t chunk_width_n);

        bool check_chunk_has_changed(const Coordinates& chunk_origin) const;

        [[nodiscard]] std::vector<std::shared_ptr<Chunk>> get_chunks() const;

    private:
        std::vector<std::shared_ptr<Chunk>> _chunks;
        std::filesystem::path _data_path;
        Coordinates _chunk_origin;

        void _load_chunk_data(float chunk_width_degrees, std::uint32_t chunk_width_n);
    };
}  // namespace fms