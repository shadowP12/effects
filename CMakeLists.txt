cmake_minimum_required (VERSION 3.8)
project ("effects")
include(CMake/PlatformConfig.cmake)
include(CMake/DependentConfig.cmake)
include(Source/Sources.cmake)
add_executable (effects ${EFFECTS_SRC})
target_include_directories(effects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_link_libraries(effects ${DependentLibs})
add_library(libglad STATIC ${GLAD_ROOT}/src/glad.c)
target_link_libraries(effects libglad)

# window's uuid
target_link_libraries(effects rpcrt4)

# file system
add_library(filesystem INTERFACE)
target_include_directories(filesystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/filesystem)
target_link_libraries(effects filesystem)

include(CMake/InstallConfig.cmake)