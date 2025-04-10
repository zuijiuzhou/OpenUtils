cmake_minimum_required(VERSION 3.10)

set(CURRENT_TARGET_NAME EnvVarEditor)

FILE(GLOB HEADER_FILES "*.h")
FILE(GLOB SRC_FILES "*.cpp")
FILE(GLOB RC_FILES "*.rc")

add_executable(${CURRENT_TARGET_NAME} ${HEADER_FILES} ${SRC_FILES} ${RC_FILES})

target_link_libraries(${CURRENT_TARGET_NAME} PRIVATE
	comm
)

target_compile_definitions(${CURRENT_TARGET_NAME} PRIVATE
	OOSU_COMM_STATIC
)

# set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")