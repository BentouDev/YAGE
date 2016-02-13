## Simple game engine
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Add SDL2
add_subdirectory(Dependencies/SDL2 EXCLUDE_FROM_ALL)
include_directories(${SDL2_INCLUDE_DIRS})

# Add GLM
add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)
include_directories(${GLM_INCLUDE_DIRS})

# Add GoogleTest
add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add CppLinq
include_directories(Dependencies/CppLinq/CppLinq)

## TODO: Add SDL2, generate OpenGL header, cppLinq

# Include all engine modules
enable_testing()
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)

add_library(Volkhvy)
target_link_libraries(Volkhvy Core Parser Script)