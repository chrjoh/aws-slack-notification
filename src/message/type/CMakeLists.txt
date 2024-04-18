SET(SCRS
cloudwatch.cpp 
inspector.cpp 
guardduty.cpp 
awshealth.cpp
)

add_library(messagetype ${SCRS})
target_include_directories(messagetype PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(messagetype PUBLIC nlohmann_json::nlohmann_json message)