#pragma once

#include <filesystem>

namespace fms {
    class FileReader {
    public:
        explicit FileReader(std::filesystem::path file_in);

        void read_file();

    private:
        std::filesystem::path file;
    };
}
