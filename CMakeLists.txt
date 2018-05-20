## Yet Another Game Engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# detect if YAGE is being bundled,
# disable testsuite in that case
if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

project(YAGE VERSION 0.1 LANGUAGES CXX C)

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
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)

# Add pThreads
find_package(Threads)

# Add GLFW
yage_add_dependency(Dependencies/GLFW)

# Add GLM
yage_add_dependency(Dependencies/GLM)

# Add Assimp
yage_add_dependency(Dependencies/Assimp)

# Add Spdlog
yage_add_dependency(Dependencies/Spdlog)

# Add Json
yage_add_dependency(Dependencies/Json)

# Add Utf8
yage_add_dependency(Dependencies/Utf8)

# Add CppLinq
yage_add_dependency(Dependencies/CppLinq)

# Add LodePng
yage_add_dependency(Dependencies/Lodepng)

# Add Valgrind support for memory debugging
if(YAGE_VALGRIND)
    find_package(Valgrind)
endif(YAGE_VALGRIND)

#============================#
# Tests                      #
#============================#
if(YAGE_BUILD_TESTS)

    # Add codecov scripts module to cmake
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules/CMake-codecov/cmake" ${CMAKE_MODULE_PATH})
    find_package(codecov REQUIRED)

    yage_add_dependency(Dependencies/Catch2)
    yage_add_dependency(Dependencies/Trompeloeil)

endif()

#============================#
# Engine Library             #
#============================#
# Include all engine modules
enable_testing()
add_subdirectory(Source/Engine)

target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Script YAGE::Core)

#============================#
# Editor                     #
#============================#
if(YAGE_BUILD_EDITOR)

    # Add QT5 when building Editor
    find_package(Qt5Widgets REQUIRED)

    # Additional windows specific features
    if(WIN32)
        find_package(Qt5WinExtras)
    endif(WIN32)

    # Include Editor
    add_subdirectory(Source/Editor)

endif(YAGE_BUILD_EDITOR)

#============================#
# Post Build                 #
#============================#

# Create log output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../Log)

# Copy configuration
configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPYONLY)

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()
