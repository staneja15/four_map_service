#pragma once

#include <filesystem>

#include <osmium/area/multipolygon_manager.hpp>
#include <osmium/area/assembler.hpp>

#include "osmium/geom/wkt.hpp"

namespace fms {
    namespace handler {
        class WKTDump : public osmium::handler::Handler {
        public:
            void area(const osmium::Area& area);

            std::vector<std::string> get_data();

        private:
            osmium::geom::WKTFactory<> _factory;
            std::vector<std::string>   _data;
        };

    }

    class FileReader {
    public:
        explicit FileReader(std::filesystem::path file_in);

        void read_file();

        std::vector<std::string> read_area_data(const osmium::TagsFilter& filter) const;

    private:
        std::filesystem::path file;
        osmium::memory::Buffer _buffer;
    };
}
