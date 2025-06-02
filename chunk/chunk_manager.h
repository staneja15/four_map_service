#pragma once

#include "chunk.h"

#include "utils/bounds.h"

namespace fms {
    class ChunkManager {
    public:
        explicit ChunkManager(const std::filesystem::path& data_directory);

        void load_chunk_data();

        [[nodiscard]] std::vector<std::shared_ptr<Chunk>> get_chunks() const;

    private:
        std::vector<std::shared_ptr<Chunk>> _chunks;
        std::filesystem::path _data_path;
    };
}  // namespace fms