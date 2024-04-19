cmake_minimum_required(VERSION 3.5)

if("${BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_BUILD_TYPE Release)
else()
    set(CMAKE_BUILD_TYPE Debug)
endif()

unset(BUILD_TYPE CACHE)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_INSTALL_OPTIONS "--allow-unsupported")
set(CMAKE_CXX_STANDARD 17)
project(aws-slack-notification LANGUAGES CXX)
enable_testing()

find_package(aws-lambda-runtime CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest CONFIG REQUIRED)
add_subdirectory(src/http)
add_subdirectory(src/message)
add_subdirectory(src/message/type)
add_subdirectory(src/env)
add_subdirectory(src/handler)
add_subdirectory(test)


add_executable(${PROJECT_NAME} "src/main.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC env handler nlohmann_json::nlohmann_json AWS::aws-lambda-runtime  )
aws_lambda_package_target(${PROJECT_NAME})