SET(SCRS
environment.cpp
)

add_library(env ${SCRS})
target_include_directories(env PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(env PUBLIC nlohmann_json::nlohmann_json http)