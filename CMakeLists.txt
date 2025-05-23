cmake_minimum_required(VERSION 3.10)

project(gs-builder)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(
    src
    ${GSTREAMER_INCLUDE_DIRS})

link_directories(${GSTREAMER_LIBRARY_DIRS})
# add_definitions(${GSTREAMER_CFLAGS_OTHER})


add_executable(${PROJECT_NAME}
    src/main.cpp
)   

target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES})
