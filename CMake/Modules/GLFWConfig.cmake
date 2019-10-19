# GLFW_INCLUDE_DIR
# GLFW_LIBS
# GLFW_DYNLIBS

set(GLFW_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glfw)

set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(GLFW_LIBS ${GLFW_ROOT}/lib/Debug/glfw3dll.lib)
	set(GLFW_DYNLIBS ${GLFW_ROOT}/lib/Debug/glfw3.dll)
ELSE()
	set(GLFW_LIBS ${GLFW_ROOT}/lib/Release/glfw3dll.lib)
	set(GLFW_DYNLIBS ${GLFW_ROOT}/lib/Release/glfw3.dll)
ENDIF()
