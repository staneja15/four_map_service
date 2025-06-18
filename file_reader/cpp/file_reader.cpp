#include "file_reader.h"

#include <exception>

#include <osmium/io/any_input.hpp>
#include <osmium/util/progress_bar.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/visitor.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/dynamic_handler.hpp>

// The type of index used. This must match the include file above
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;

// The location handler always depends on the index type
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

namespace fms {
    namespace handler {
        void WKTDump::area(const osmium::Area& area)  {
            try {
                _data.emplace_back(_factory.create_multipolygon(area));
            } catch (const osmium::geometry_error& e) {
                std::cout << "GEOMETRY ERROR: " << e.what() << "\n";
            }
        }

        std::vector<std::string> WKTDump::get_data() {
            return _data;
        }
    }  // namespace handler

    FileReader::FileReader(std::filesystem::path file_in)
        : file(std::move(file_in))
    { }

    void FileReader::read_file() {
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

    std::vector<std::string> FileReader::read_area_data(const osmium::TagsFilter& filter) const {
        // Create a handler with the WKT (Well Known Text) geometry format
        // osmium::handler::DynamicHandler handler;
        // handler.set<handler::WKTDump>();

        handler::WKTDump handler;

        const osmium::io::File input_file{ file };
        osmium::area::Assembler::config_type assembler_config { };
        assembler_config.ignore_invalid_locations = true;

        osmium::area::MultipolygonManager<osmium::area::Assembler> mp_manager {assembler_config, filter};
        osmium::relations::read_relations(input_file, mp_manager);

        index_type index;
        location_handler_type location_handler{index};

        location_handler.ignore_errors();

        osmium::io::Reader reader{input_file};
        osmium::apply(reader, location_handler, mp_manager.handler([&handler](osmium::memory::Buffer&& buffer) {
            osmium::apply(buffer, handler);
        }));
        reader.close();

        // Print output to console
        for (auto it = mp_manager.buffer().begin(); it != mp_manager.buffer().end(); ++it) {
            it.print(std::cout);
        }
        std::cerr << "Memory:\n";
        osmium::relations::print_used_memory(std::cerr, mp_manager.used_memory());

        return handler.get_data();
    }
}  // namespace fms
