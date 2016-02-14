## Simple game engine
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Add ./CMake to modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add GLFW
add_subdirectory(Dependencies/GLFW EXCLUDE_FROM_ALL)
include_directories(Dependencies/GLFW/include)

# Add GLM
add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)
include_directories(${GLM_INCLUDE_DIRS})

# Add GoogleTest
add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add CppLinq
include_directories(Dependencies/CppLinq/CppLinq)

## TODO: Add generate OpenGL header

# Include all engine modules
enable_testing()
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)

add_library(Volkhvy)
target_link_libraries(Volkhvy Core Parser Script)
