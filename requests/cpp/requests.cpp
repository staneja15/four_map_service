#include "requests.h"

#include <cstdlib>
#include <thread>

#include "extern/nlohmann/json.hpp"
#include "extern/polyline_encoder/polyline_encoder.h"

using json = nlohmann::json;

namespace fms {
    /// Starts a local valhalla service which is required for all routing and elevation requests
    void Requests::start_service() {
        // Check if the service is already running
        if (!_ping()) {
            // Start the service
            system("valhalla_service /opt/valhalla/valhalla.json&");  // & symbol runs the service in the background so it is non-blocking
            system("sleep 0.5");  // Give the service a short period to get setup before querying

            if (!_ping()) {
                // If we still can't ping after running the service again, something has gone wrong.
                throw std::runtime_error("Failed to start the valhalla service.");
            }

            _update_service_info();
        }
    }

    void Requests::download_all_maps(const std::filesystem::path& data_path, const float chunk_width_degrees, const std::uint32_t chunk_width_units, const bool overwrite_data) {
        start_service();

        for (const auto& map_bounds : get_bounds()) {
            MapGenerationInfo map_info = {
                .bounds = map_bounds,
                .chunk_width_degrees = chunk_width_degrees,
                .width = chunk_width_units,
                .padding = true
            };

            if (exists(data_path / map_bounds.to_string()) && !overwrite_data) {
                // Early exist if map data is already present and `overwrite_data` is set to `false`
                continue;
            }

            std::vector<Chunk> chunks = get_elevation(map_info);

            for (const auto& chunk : chunks) {
                auto path = data_path / map_bounds.to_string();
                chunk.write_elevation_data(path, overwrite_data);
            }
        }
    }

    /// Requests elevation information from valhalla based on the map generation info provided in the parameters list.
    std::vector<Chunk> Requests::get_elevation(const MapGenerationInfo& map_gen_info) {
        constexpr double error = Chunk::error;
        std::vector<Chunk> chunks = {};

        float one_unit = 0.0f;
        if (map_gen_info.padding) {
            one_unit = map_gen_info.chunk_width_degrees / static_cast<float>(map_gen_info.width);
        }

        // Create N chunks based on the number of "chunk widths" that will fit inside the given bounds.
        double lat = map_gen_info.bounds.min.lat;
        while (lat < map_gen_info.bounds.max.lat - error) {

            double lon = map_gen_info.bounds.min.lon;
            while (lon < map_gen_info.bounds.max.lon - error) {
                // Create the chunk
                const auto bounds_min = Coordinates(lon, lat);
                const auto bounds_max = Coordinates(lon + map_gen_info.chunk_width_degrees + one_unit, lat + map_gen_info.chunk_width_degrees + one_unit);
                auto bounds = Bounds(std::vector<Coordinates>{bounds_min, bounds_max});
                chunks.emplace_back(map_gen_info.width, bounds);

                lon += map_gen_info.chunk_width_degrees;
            }
            lat += map_gen_info.chunk_width_degrees;
        }

        // Request the height data for each chunk
        std::vector<std::thread> threads;
        threads.reserve(chunks.size());
        for (auto& chunk: chunks) {
            threads.emplace_back([&chunk]() {
                // Format the request by transforming the coordinate positions to an encoded polyline
                const std::string elevation_request = _format_elevation_request(chunk.generate_chunk_coordinates());

                // Send the http post request
                cpr::Response response = cpr::Post(cpr::Url{"http://localhost:8002/height"},
                                      cpr::Body{elevation_request},
                                      cpr::Header{{"Content-Type", "Application/json"}});

                // Convert null values into 0.0f
                auto height_array = json::parse(response.text)["height"];
                std::ranges::for_each(height_array, [](const nlohmann::json& height) {
                    return height.is_null() ? 0.0f : static_cast<float>(height);
                });

                chunk.height_data = static_cast<std::vector<float>>(height_array);
            });
        };

        for (auto& thread: threads) {
            thread.join();
        }

        return chunks;
    }

    std::vector<Bounds> Requests::get_bounds() {
        return _bounds;
    }

    /// Pings the valhalla service on port 8002
    bool Requests::_ping() {
        _latest_response = cpr::Get(
            cpr::Url{"http://localhost:8002/status"},
            cpr::Body{R"({"verbose": true})"},
            cpr::Header{{"Content-Type", "Application/json"}}
        );

        return _latest_response.status_code == 200;
    }

    /// Updates the polygons and bounds from the status.json response of the valhalla service
    ///   - There will be 1 polygon and 1 bounds update per loaded osm data file
    void Requests::_update_service_info() {
        if (!_ping()) {
            return;
        }

        auto status = json::parse(_latest_response.text);
        for (const auto& feat : status["bbox"]["features"]) {
            std::vector<Coordinates> current_polygon = {};
            current_polygon.reserve(feat["geometry"]["coordinates"][0].size());

            for (const auto& coords : feat["geometry"]["coordinates"][0]) {
                current_polygon.emplace_back(coords);
            }

            _polygons.emplace_back(current_polygon);
            _bounds.emplace_back(current_polygon);
        }
    }

    /// Converts a vector of Coordinates into an encoded polyline string which is an optimal encoding used for requesting information with lat/lon coordinates via http.
    ///   - polyline encoding is extremely fast compared to other methods, and is far smaller compared to a human-readable json formatted request. This is
    ///     especially important when sending http requests as a smaller payload is always preferred.
    std::string Requests::_format_elevation_request(const std::vector<Coordinates>& coords) {
        //   Format the http elevation request from the input data stream
        constexpr std::int8_t precision = 6;

        gepaf::PolylineEncoder<precision> encoder;

        for (const auto& [lon, lat] : coords) {
            encoder.addPoint(lat, lon);
        }

        nlohmann::json payload;
        payload["height_precision"] = 2;
        payload["shape_format"] = precision;
        payload["encoded_polyline"] = encoder.encode();

        return payload.dump();
    }
}  // namespace fms
