## Yet Another Game Engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(YAGE VERSION 0.1 LANGUAGES CXX C)

# Supress warning for dependencies with old Cmake files
cmake_policy(PUSH)
cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0048 OLD)

option(YAGE_BUILD_TESTS "Build Tests" ON)
option(YAGE_BUILD_EDITOR "Build Editor" ON)
option(YAGE_VALGRIND "Build with valgrind headers" ON)

# emit additional metadata for debug builds
include(CMakeToolsHelpers OPTIONAL)

# enforce warnings
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif ( MSVC )

# Add modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})

# Add Vulkan
find_package(Vulkan)

# Add OpenGl
find_package(OpenGL)

# Add pThreads
find_package (Threads)

# Add GLFW
find_package(GLFW)
include_directories(${GLFW_INCLUDE}})

# Add GLM
find_package(GLM)
include_directories(${GLM_INCLUDE}})

# Add Assimp
find_package(Assimp)
include_directories(${ASSIMP_INCLUDE}})

# Add Spdlog
find_package(Spdlog)
include_directories(${SPDLOG_INCLUDE}})

# Add Json
add_subdirectory(Dependencies/Json EXCLUDE_FROM_ALL)
include_directories(Dependencies/Json/src)

# Add Utf8
include_directories(Dependencies/Utf8/source)
add_library(utf8 Dependencies/Utf8/source/utf8.h Dependencies/Utf8/source/utf8/checked.h Dependencies/Utf8/source/utf8/core.h Dependencies/Utf8/source/utf8/unchecked.h)
set_target_properties(utf8 PROPERTIES LINKER_LANGUAGE CXX)

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
    find_package(GTest)
    find_package(GMock)
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
target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Script YAGE::Core)

configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPY_ONLY)

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()
