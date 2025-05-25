set(target utils)

add_library(
    ${target}
    STATIC
    .
)

set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    ${target}
    PUBLIC
    fmt::fmt
)