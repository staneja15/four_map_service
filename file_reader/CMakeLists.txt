set(target file_reader)

add_library(
    ${target}
    STATIC
    cpp/file_reader.cpp
)

target_link_libraries(
    ${target}
    PUBLIC
    osmium_dependencies
)

target_include_directories(
    ${target}
    PRIVATE
    ${FMS_ROOT_DIR}
    ${FMS_ROOT_DIR}/extern
)