if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()
include_directories(third_party/glad/include)
include_directories(third_party/glfw/include)
include_directories(third_party/glm)
include_directories(third_party/stb)
include_directories(third_party/fftw)
include_directories(${CMAKE_SOURCE_DIR})

add_library(glad STATIC third_party/glad/src/glad.c)

add_executable (effects 
				main.cpp
				utils/gl_utils.cpp
				utils/utils.cpp
				core/ocean_rendering/ocean_rendering.cpp)
target_link_libraries(effects glad)

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Debug/glfw3dll.lib)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Debug/glfw3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
ELSE()
	target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Release/glfw3dll.lib)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw/lib/Debug/glfw3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
ENDIF()

#fftw
target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3-3.lib)
target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3f-3.lib)
target_link_libraries(effects ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3l-3.lib)

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3f-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3l-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
ELSE()
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3f-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/fftw/libfftw3l-3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
ENDIF()