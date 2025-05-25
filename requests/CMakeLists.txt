set(target requests)

add_library(
    ${target}
    STATIC
    cpp/requests.cpp
)

target_include_directories(
    ${target}
    PRIVATE
    ${FMS_ROOT_DIR}
    ${FMS_ROOT_DIR}/extern
)

target_link_libraries(
    ${target}
    PRIVATE
    cpr::cpr

    utils
)