SET(SCRS
slack.cpp 
)

add_library(message ${SCRS})
target_include_directories(message PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(message PUBLIC nlohmann_json::nlohmann_json messagetype)