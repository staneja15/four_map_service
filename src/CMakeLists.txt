# CPR dependecies
find_package(cpr)
find_package(OpenSSL REQUIRED)

# Osmium dependencies
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(EXPAT REQUIRED)
find_package(Threads REQUIRED)

# Google Test
find_package(GTest REQUIRED)

add_executable(
    test_four_map_service
    test/test.cpp
)

target_link_libraries(
    test_four_map_service
    GTest::gtest_main
)
