cmake_minimum_required(VERSION 3.10)

project(gs-builder)

include(FetchContent)

set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_BUILD_PIC ON)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

find_package(yaml-cpp REQUIRED)

include_directories(
    src
    ${GSTREAMER_INCLUDE_DIRS}

    # tests
    tests
)

link_directories(${GSTREAMER_LIBRARY_DIRS})

# App

add_library(${PROJECT_NAME} SHARED
    src/main.cpp

    # Graph
    src/graph/GraphNode.cpp
    src/graph/YamlGraphBuilder.cpp

    src/wrapper/gstcpp.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${GSTREAMER_LIBRARIES}
    yaml-cpp
    spdlog::spdlog
)

# Tests

add_executable(${PROJECT_NAME}-tests
    tests/graph/YamlGraphBuilderTests.cpp
    tests/wrapper/WrapperTest.cpp
    tests/main.cpp
)

target_link_libraries(${PROJECT_NAME}-tests
    ${GSTREAMER_LIBRARIES}
    ${PROJECT_NAME}
    gtest
)
