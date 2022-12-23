cmake_minimum_required(VERSION 3.10)
project(recorder)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})

add_executable(recorder stream.cpp)
target_link_libraries(recorder ${GSTREAMER_LIBRARIES})
