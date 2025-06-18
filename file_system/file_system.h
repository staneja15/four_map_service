#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

#include <fmt/format.h>

#define OSM_FILE_DIR "/opt/valhalla/"

namespace fs = std::filesystem;

namespace fms {
    std::vector<fs::path> get_osm_files();

    template <typename T>
    void write_to_binary(const std::filesystem::path& filename, const std::vector<T>& vec) {
        // If the file dir does not exist, make it.
        std::string cmd = fmt::format("mkdir -p {0}", filename.parent_path().c_str());
        system(cmd.c_str());

        std::ofstream output (filename, std::ios::out | std::ios::binary);
        output.write(reinterpret_cast<const char*>(vec.data()), sizeof(T) * vec.size());
        output.close();
    };

    template <typename T>
    void read_binary(const std::filesystem::path& filename, std::vector<T>& data_out) {
        T data;
        std::ifstream fin (filename, std::ifstream::binary);
        while (fin.read(reinterpret_cast<char*>(&data), sizeof(T))) {
            data_out.emplace_back(data);
        }
    }
}