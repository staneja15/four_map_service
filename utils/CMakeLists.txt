set(target fms_utils)

add_library(
    ${target}
    STATIC
    cpp/bounds.cpp
    cpp/chunk.cpp
    cpp/coordinates.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${target}
    PUBLIC
    fmt::fmt
)