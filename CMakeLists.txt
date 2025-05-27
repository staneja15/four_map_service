set(FMS_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# CPR dependencies
find_package(cpr)
find_package(OpenSSL REQUIRED)

# Osmium dependencies
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(EXPAT REQUIRED)
find_package(Threads REQUIRED)

add_library(
    osmium_dependencies
    INTERFACE
)

target_link_libraries(
    osmium_dependencies
    INTERFACE
    expat
    BZip2::BZip2
    ZLIB::ZLIB
    Threads::Threads
)

# fmt lib
find_package(fmt REQUIRED)


# Google Test
find_package(GTest REQUIRED)

# Add project sub directories
add_subdirectory(file_system)
add_subdirectory(file_reader)
add_subdirectory(requests)
add_subdirectory(utils)

# Create the test executable
add_executable(
    test_four_map_service
    test/test.cpp
)

target_link_libraries(
    test_four_map_service
    PRIVATE
    GTest::gtest_main

    file_system
    file_reader
    requests
)

target_include_directories(
    test_four_map_service
    PRIVATE
    ${FMS_ROOT_DIR}
)

# Create the map service library
add_library(
    four_map_service
    INTERFACE
)

target_link_libraries(
    four_map_service
    INTERFACE
    file_system
    file_reader
    requests
    utils
)

target_include_directories(
    four_map_service
    INTERFACE
    ${FMS_ROOT_DIR}
)

add_library(four::map_service ALIAS four_map_service)

