cmake_minimum_required(VERSION 3.10)

include_directories(${THIRD_ARGPARSE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Comm)
add_subdirectory(EnvVarEditor)

