cmake_minimum_required(VERSION 3.10)

set(CURRENT_TARGET_NAME ouComm)

add_definitions(-DOOSU_COMM_STATIC)

FILE(GLOB_RECURSE HEADER_FILES "*.h")
FILE(GLOB_RECURSE SRC_FILES "*.cpp")

add_library(${CURRENT_TARGET_NAME} STATIC ${HEADER_FILES} ${SRC_FILES})

set_property(TARGET ${CURRENT_TARGET_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")



# set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")