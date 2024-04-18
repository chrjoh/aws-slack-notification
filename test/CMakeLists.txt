SET(SRCS 
main.cpp
clientTest.cpp
env/varTest.cpp
env/environmentTest.cpp
handler/handlerTest.cpp
message/cloudwatchTest.cpp
message/inspectorTest.cpp
message/guarddutyTest.cpp
message/awshealthTest.cpp
)


add_executable(unit_tests  ${SRCS} )
target_link_libraries(
  unit_tests PUBLIC message messagetype env handler http GTest::gtest_main GTest::gmock GTest::gtest
)
include(GoogleTest)
gtest_discover_tests(unit_tests)