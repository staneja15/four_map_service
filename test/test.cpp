#include "file_system/file_system.h"
#include "file_reader/file_reader.h"
#include "requests/requests.h"
#include "requests/map_generation.h"
#include "utils/bounds.h"

#include <gtest/gtest.h>

using namespace fms;

TEST(basic, test) {
    const std::vector<std::filesystem::path> files = get_osm_files();
    for (const auto& file : files) {
        auto fr = FileReader(file);
        fr.read_file();
        break;
    }
}

TEST(requests, elevation) {
    // Test that we can successfully request elevation data for a given bounds
    Requests::start_service();

    for (const auto& map_bounds : Requests::get_bounds()) {
        MapGenerationInfo map_info = {
            .bounds = map_bounds,
            .chunk_width_degrees = 0.1,  // 1 degrees lat/lon per chunk
            .width = 10                  // 100 total units within the chunk
        };

        std::vector<Chunk> chunks = Requests::get_elevation(map_info);

        for (const auto& chunk : chunks) {
            auto path = std::filesystem::path("/opt/four_map_service/data/") / map_bounds.to_string();
            chunk.write_elevation_data(path);
        }
    }
}

TEST(requests, read_elevation_data) {
    float f;
    std::ifstream fin ("/opt/four_map_service/data/height.dat", std::ifstream::binary);
    while (fin.read(reinterpret_cast<char*>(&f), sizeof(float)))
        std::cout << f << '\n';
};

