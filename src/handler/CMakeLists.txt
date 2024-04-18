SET(SCRS
handler.cpp 
)

add_library(handler ${SCRS})
target_include_directories(handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(handler PUBLIC nlohmann_json::nlohmann_json ${CURL_LIBRARIES} message messagetype http env)