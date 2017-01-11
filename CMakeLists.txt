## Yet Another Game Engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(YAGE VERSION 0.1 LANGUAGES CXX)

# Supress warning for dependencies with old Cmake files
cmake_policy(PUSH)
cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0048 OLD)

option(YAGE_BUILD_TESTS "Build Tests" ON)
option(YAGE_BUILD_EDITOR "Build Editor" ON)
option(YAGE_VALGRIND "Build with valgrind headers" ON)

# emit additional metadata for debug builds
include(CMakeToolsHelpers OPTIONAL)

# Add modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})

# Add Vulkan
find_package(Vulkan)

# Add OpenGl
find_package(OpenGL)

# Add GLFW
add_subdirectory(Dependencies/GLFW EXCLUDE_FROM_ALL)
include_directories(Dependencies/GLFW/include)

# Add GLM
add_subdirectory(Dependencies/GLM EXCLUDE_FROM_ALL)
include_directories(${GLM_INCLUDE_DIRS})

# Add Assimp
add_subdirectory(Dependencies/Assimp EXCLUDE_FROM_ALL)
include_directories(Dependencies/Assimp/include)

# Add Spdlog
add_subdirectory(Dependencies/Spdlog EXCLUDE_FROM_ALL)
include_directories(Dependencies/Spdlog/include)

# Add Json
add_subdirectory(Dependencies/Json EXCLUDE_FROM_ALL)
include_directories(Dependencies/Json/src)

# Add CppLinq
include_directories(Dependencies/CppLinq)

# Add LodePng
include_directories(Dependencies/Lodepng)
add_library(lodepng Dependencies/Lodepng/lodepng.cpp Dependencies/Lodepng/lodepng.h)

if(YAGE_BUILD_TESTS)
    # Add codecov scripts module to cmake
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
    find_package(codecov REQUIRED)

    # Add GoogleTest
    set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" )
    add_subdirectory(Dependencies/GoogleTest EXCLUDE_FROM_ALL)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
endif()

# Add QT5 when building Editor
if(YAGE_BUILD_EDITOR)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5WinExtras)
endif(YAGE_BUILD_EDITOR)

# Add Valgrind support for memory debugging
if(YAGE_VALGRIND)
    find_package(Valgrind)
endif(YAGE_VALGRIND)

# Revert to standard policy
cmake_policy(POP)

# Include all engine modules
enable_testing()
add_subdirectory(Source/Utils)
add_subdirectory(Source/Core)
add_subdirectory(Source/Script)

# Include Editor
if(YAGE_BUILD_EDITOR)
    add_subdirectory(Source/Editor)
endif(YAGE_BUILD_EDITOR)

add_library(YAGE INTERFACE)
target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Core YAGE::Script)

configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPY_ONLY)

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()
