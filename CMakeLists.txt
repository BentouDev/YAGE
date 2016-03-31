## Simple game engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Supress warning for dependencies with old Cmake files
cmake_policy(PUSH)
cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0048 OLD)

# Add codecov scripts module to cmake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
find_package(codecov REQUIRED)

# Add Vulkan
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Vulkan" ${CMAKE_MODULE_PATH})
find_package(Vulkan)

# Add OpenGl
find_package(OpenGL)

# Add GLFW
add_subdirectory(Dependencies/GLFW EXCLUDE_FROM_ALL)
include_directories(Dependencies/GLFW/include)

# Add GLM
add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)
include_directories(${GLM_INCLUDE_DIRS})

# Add GoogleTest
set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" )
add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

# Add Spdlog
add_subdirectory(Dependencies/Spdlog EXCLUDE_FROM_ALL)
include_directories(Dependencies/Spdlog/include)

# Add Json
add_subdirectory(Dependencies/Json EXCLUDE_FROM_ALL)
include_directories(Dependencies/Json/src)

# Add CppLinq
include_directories(Dependencies/CppLinq)

# Revert to standard policy
cmake_policy(POP)

# Include all engine modules
enable_testing()
add_subdirectory(Source/Utils)
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)

add_library(Volkhvy INTERFACE)
target_link_libraries(Volkhvy INTERFACE Volkhvy::Utils Volkhvy::Core Volkhvy::Parser Volkhvy::Script)

coverage_evaluate()