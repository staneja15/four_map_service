set(target file_system)

add_library(
    ${target}
    STATIC
    cpp/file_system.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
