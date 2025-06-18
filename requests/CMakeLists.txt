set(target requests)

add_library(
    ${target}
    STATIC
    cpp/requests.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${target}
    PRIVATE
    cpr::cpr

    chunk
    fms_utils
    file_reader
    file_system
)