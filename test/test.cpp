#include "file_system/file_system.h"
#include "file_reader/file_reader.h"
#include "requests/requests.h"

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
    Requests::download_all_maps("/opt/four_map_service/data/");
}

TEST(requests, read_elevation_data) {
    float f;
    std::ifstream fin ("/opt/four_map_service/data/height.dat", std::ifstream::binary);
    while (fin.read(reinterpret_cast<char*>(&f), sizeof(float)))
        std::cout << f << '\n';
};

