set(target chunk)

add_library(
    ${target}
    STATIC
    cpp/chunk.cpp
    cpp/chunk_manager.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${target}
    PUBLIC
    fms_utils
)