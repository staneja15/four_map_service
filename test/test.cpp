#include "file_system/file_system.h"
#include "file_reader/file_reader.h"
#include "requests/requests.h"

#include <gtest/gtest.h>

using namespace fms;

/// Reads all map files in the OSM_FILES dir using libosmium
TEST(requests, read_map_files) {
    const std::vector<std::filesystem::path> files = get_osm_files();
    for (const auto& file : files) {
        auto fr = FileReader(file);
        fr.read_file();
        break;
    }
}
