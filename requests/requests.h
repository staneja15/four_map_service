#pragma once

#include "map_generation.h"
#include "utils/bounds.h"
#include "chunk/chunk.h"

#include <cpr/cpr.h>

namespace fms {
    class Requests {
    public:
        static void start_service();

        static void download_all_maps(const std::filesystem::path& data_path, float chunk_width_degrees, std::uint32_t chunk_width_units, bool overwrite_data=false);

        static std::vector<Chunk> get_elevation(const MapGenerationInfo& map_gen_info);

        static std::vector<Bounds> get_bounds();

    private:
        static inline cpr::Response _latest_response = {};
        static inline std::vector<std::vector<Coordinates>> _polygons = {};
        static inline std::vector<Bounds> _bounds = {};

        static bool _ping();

        static void _update_service_info();

        static std::string _format_elevation_request(const std::vector<Coordinates>& coords);

    };
}  // namespace fms