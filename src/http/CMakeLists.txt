add_library(http client.cpp)
target_include_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(http PUBLIC ${CURL_LIBRARIES} nlohmann_json::nlohmann_json )