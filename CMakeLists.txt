## Yet Another Game Engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# detect if YAGE is being bundled,
# disable testsuite in that case
if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

project(YAGE VERSION 0.1 LANGUAGES CXX C)

# Supress warning for dependencies with old Cmake files
cmake_policy(PUSH)

option(YAGE_BUILD_TESTS "Build Tests" ${NOT_SUBPROJECT})
option(YAGE_BUILD_EDITOR "Build Editor" ON)
option(YAGE_VALGRIND "Build with valgrind headers" ON)

# Add modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})
include (${CMAKE_CURRENT_SOURCE_DIR}/Modules/Conan.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/Modules/YageSetupDependency.cmake)

# Emit additional metadata for debug builds
include(CMakeToolsHelpers OPTIONAL)

# Enforce warnings
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif(MSVC)

#============================#
#   _  _   __    ___  ____   #
#  ( \/ ) /__\  / __)( ___)  #
#   \  / /(__)\( (_-. )__)   #
#   (__)(__)(__)\___/(____)  #
#                            #
#   Yet Another Game Engine  #
#                            #
#=============================

# Define engine library
add_library(YAGE INTERFACE)

# Add Vulkan
find_package(Vulkan)

# Add OpenGl
find_package(OpenGL)

# Add pThreads
find_package(Threads)

# Add GLFW
set (GLFW_BUILD_DOCS OFF CACHE BOOL "")
set (GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set (GLFW_BUILD_TESTS OFF CACHE BOOL "")

yage_setup_dependency(GLFW
        CONAN glfw/3.2.1.20180327@bincrafters/stable
        TARGET glfw
        SOURCE Dependencies/GLFW
        INCLUDE Dependencies/GLFW/include)

yage_include(YAGE ${GLFW_INCLUDE_DIR})

# Add GLM
yage_setup_dependency(GLM
        SOURCE Dependencies/GLM
        INCLUDE Dependencies/GLM)

yage_include(YAGE ${GLM_INCLUDE_DIR})

# Add Assimp
yage_setup_dependency(Assimp
        CONAN Assimp/0.1@bentoudev/yage
        VAR_NAME ASSIMP
        TARGET assimp
        SOURCE Dependencies/Assimp
        INCLUDE Dependencies/Assimp/include)

yage_include(YAGE ${ASSIMP_INCLUDE_DIR})

# Windows needs zlibstatic and IrrXml in assimp export target
if((NOT ${ASSIMP_FOUND}) AND (WIN32))
    if(MINGW)
        set(ASSIMP_TARGETS assimp zlibstatic)
    else()
        set(ASSIMP_TARGETS assimp zlibstatic)
    endif()
else()
    set(ASSIMP_TARGETS ${ASSIMP_TARGET})
endif()

# Add Spdlog
yage_setup_dependency(Spdlog
        VAR_NAME SPDLOG
        SOURCE Dependencies/Spdlog
        INCLUDE Dependencies/Spdlog/include)

yage_include(YAGE ${SPDLOG_INCLUDE_DIR})

# Add Json
add_subdirectory(Dependencies/Json EXCLUDE_FROM_ALL)
yage_include(YAGE Dependencies/Json/single_include)

# Add Utf8
yage_include(YAGE Dependencies/Utf8/source)
add_library(utf8 STATIC Dependencies/Utf8/source/utf8.h Dependencies/Utf8/source/utf8/checked.h Dependencies/Utf8/source/utf8/core.h Dependencies/Utf8/source/utf8/unchecked.h)
set_target_properties(utf8 PROPERTIES LINKER_LANGUAGE CXX)

# Add CppLinq
yage_include(YAGE Dependencies/CppLinq)

# Add LodePng
yage_include(YAGE Dependencies/Lodepng)
add_library(lodepng STATIC Dependencies/Lodepng/lodepng.cpp Dependencies/Lodepng/lodepng.h)

if(YAGE_BUILD_TESTS)
    # Add codecov scripts module to cmake
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
    find_package(codecov REQUIRED)

    include_directories(Dependencies/Catch2/single_include)
    include_directories(Dependencies/Trompeloeil/include)
endif()

# Add QT5 when building Editor
if(YAGE_BUILD_EDITOR)
    find_package(Qt5Widgets REQUIRED)

    # Additional windows specific features
    if(WIN32)
        find_package(Qt5WinExtras)
    endif(WIN32)
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

target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Script YAGE::Core)

configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPYONLY)

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()
