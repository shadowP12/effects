# fftw
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/FFTWConfig.cmake)
include_directories(${FFTW_INCLUDE_DIR})
set(DependentLibs ${DependentLibs} ${FFTW_LIBS})

# stb
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/STBConfig.cmake)
include_directories(${STB_INCLUDE_DIR})

# gear math
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/GEARMATHConfig.cmake)
include_directories(${GEARMATH_INCLUDE_DIR})

# glm
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/GLMConfig.cmake)
include_directories(${GLM_INCLUDE_DIR})

# glad
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/GLADConfig.cmake)
include_directories(${GLAD_INCLUDE_DIR})

# glfw
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/GLFWConfig.cmake)
include_directories(${GLFW_INCLUDE_DIR})
set(DependentLibs ${DependentLibs} ${GLFW_LIBS})

# cgltf
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules/CGLTFConfig.cmake)
include_directories(${CGLTF_INCLUDE_DIR})