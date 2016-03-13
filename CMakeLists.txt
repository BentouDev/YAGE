## Simple game engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Add codecov scripts module to cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Dependencies/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
find_package(codecov)

# Add Vulkan
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Dependencies/Vulkan" ${CMAKE_MODULE_PATH})
find_package(Vulkan)
include_directories(${VULKAN_INCLUDE_DIR})

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

# Add CppLinq
include_directories(Dependencies/CppLinq/CppLinq)

# Include all engine modules
enable_testing()
add_subdirectory(Source/Utils)
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)

add_library(Volkhvy)
target_link_libraries(Volkhvy Volkhvy::Utils Volkhvy::Core Volkhvy::Parser Volkhvy::Script ${VULKAN_LIBRARY})

coverage_evaluate()