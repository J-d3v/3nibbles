cmake_minimum_required(VERSION 3.21)
project(equipo4 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
include(CTest)
enable_testing()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.18.3
)
FetchContent_MakeAvailable(httplib)

set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(algorithms_lib
    src/algorithms.cpp
)
target_include_directories(algorithms_lib PUBLIC include)

add_executable(equipo4 main.cpp)
target_link_libraries(equipo4 PRIVATE algorithms_lib)

add_executable(server src/server.cpp)
target_link_libraries(server PRIVATE algorithms_lib httplib::httplib nlohmann_json::nlohmann_json)

add_executable(unit_tests
    tests/unit/test_algorithms.cpp
)
target_link_libraries(unit_tests PRIVATE algorithms_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests TEST_PREFIX unit.)
