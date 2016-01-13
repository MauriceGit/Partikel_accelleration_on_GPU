cmake_minimum_required(VERSION 2.8)

project(particles)

SET(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${CMAKE_SOURCE_DIR} ${GLFW_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCEFILES ${CMAKE_SOURCE_DIR}/*.c)

add_executable(particles ${SOURCEFILES})
target_link_libraries(particles GLU GL GLEW m ${GLFW_LIBRARIES})