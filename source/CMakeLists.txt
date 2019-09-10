include_directories(third_party/glad/include)
include_directories(third_party/glfw/include)
include_directories(third_party/glm)
include_directories(third_party/stb)

add_library(glad STATIC third_party/glad/src/glad.c)

add_executable (effects main.cpp)
target_link_libraries(effects glad)

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Debug/glfw3dll.lib)
ELSE()
	target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Release/glfw3dll.lib)
ENDIF()