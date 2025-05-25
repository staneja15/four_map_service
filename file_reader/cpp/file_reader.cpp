#include "file_reader/file_reader.h"

#include <exception>
#include <iostream> // for std::cerr

// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>
#include <osmium/util/progress_bar.hpp>

fms::FileReader::FileReader(std::filesystem::path file_in)
    : file(std::move(file_in))
{ }

void fms::FileReader::read_file() {
    try {
        // The Reader is initialized here with an osmium::io::File, but could
        // also be directly initialized with a file name.
        osmium::io::Reader reader { file };
        osmium::ProgressBar progress { reader.file_size(), true };

        // OSM data comes in buffers, read until there are no more.
        while (const osmium::memory::Buffer buffer = reader.read()) {
            progress.update(reader.offset());
        }

        // You do not have to close the Reader explicitly, but because the
        // destructor can't throw, you will not see any errors otherwise.
        reader.close();
    } catch (const std::exception& e) {
        // All exceptions used by the Osmium library derive from std::exception.
        std::cerr << e.what() << '\n';
    }
}
