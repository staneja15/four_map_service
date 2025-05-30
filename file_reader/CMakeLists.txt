set(target file_reader)

add_library(
    ${target}
    STATIC
    cpp/file_reader.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${target}
    PUBLIC
    osmium_dependencies
)
