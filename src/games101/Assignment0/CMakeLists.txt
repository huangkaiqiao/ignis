cmake_minimum_required (VERSION 2.8.11)
project (Transformation)

# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

add_executable (Transformation main.cpp)
