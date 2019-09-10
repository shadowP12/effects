cmake_minimum_required (VERSION 3.8)
project ("effects")
IF (CMAKE_BUILD_TYPE MATCHES "Debug")
	message("Debug")
ELSE()
	message("Release")
ENDIF()

add_subdirectory ("source")