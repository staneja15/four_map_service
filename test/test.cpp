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
    }
}

/// Reads the area data for each of the map fies in the OSM_FILES dir using libosmium
TEST(requests, read_area_data) {
    const std::vector<std::filesystem::path> files = get_osm_files();
    for (const auto& file : files) {
        auto fr = FileReader(file);

        osmium::TagsFilter filter;
        filter.add_rule(true, "natural", "water");
        fr.read_area_data(filter);
    }
}
