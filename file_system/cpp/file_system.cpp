#include "file_system/file_system.h"

std::vector<fs::path> fms::get_osm_files() {
    std::vector<fs::path> osm_files = {};

    for (const auto & entry : fs::directory_iterator(OSM_FILE_DIR)) {
        if (entry.path().extension() == ".pbf") {
            osm_files.emplace_back(entry.path());
        }
    }

    return osm_files;
}
