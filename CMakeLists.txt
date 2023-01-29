cmake_minimum_required(VERSION 3.0.0)
project(shitcraft VERSION 0.1.0)

find_package(SFML REQUIRED network audio graphics window system)

include(CTest)
enable_testing()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
add_executable(shitcraft ./src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_link_libraries(shitcraft PRIVATE GL sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
