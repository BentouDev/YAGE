## Simple game engine
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Volkhvy VERSION 0.1 LANGUAGES CXX)

# Add GLM
#find_package(Glm REQUIRED)
add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)
set_property(TARGET glm PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

# Add GoogleTest
add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Include all modules
enable_testing()
add_subdirectory(Source/Core)
add_subdirectory(Source/Parser)
add_subdirectory(Source/Script)