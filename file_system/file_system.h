#pragma once

#include <vector>
#include <filesystem>

#define OSM_FILE_DIR "/opt/valhalla/"

namespace fs = std::filesystem;

namespace fms {
    std::vector<fs::path> get_osm_files();
}