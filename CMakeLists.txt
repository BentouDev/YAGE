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

# Add modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})
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

# Add Vulkan
find_package(Vulkan)

# Add OpenGl
find_package(OpenGL)

# Add pThreads
find_package (Threads)

# Add GLFW
yage_setup_dependency(GLFW
        TARGET glfw
        SOURCE Dependencies/GLFW
        INCLUDE Dependencies/GLFW/include)

include_directories(${GLFW_INCLUDE}})

# Add GLM
yage_setup_dependency(GLM
        SOURCE Dependencies/GLM
        INCLUDE Dependencies/GLM)

include_directories(${GLM_INCLUDE}})

# Add Assimp
yage_setup_dependency(Assimp
        VAR_NAME ASSIMP
        TARGET assimp
        SOURCE Dependencies/Assimp
        INCLUDE Dependencies/Assimp/include)

include_directories(${ASSIMP_INCLUDE}})

# Windows needs zlibstatic and IrrXml in assimp export target
if(WIN32)
    set(ASSIMP_TARGETS assimp zlibstatic)
else()
    set(ASSIMP_TARGETS ${ASSIMP_TARGET})
endif()

# Add Spdlog
yage_setup_dependency(Spdlog
        VAR_NAME SPDLOG
        SOURCE Dependencies/Spdlog
        INCLUDE Dependencies/Spdlog/include)

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

    # Fix MinGW link errors
    set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")

    # Add GoogleMock, which source has implicit GoogleTest dependency
    yage_setup_dependency(GMock
            TARGET gmock
            VAR_NAME GMOCK
            SOURCE Dependencies/GoogleTest)

    message("TEST " ${GMOCK_FOUND} " s " ${GMock_FOUND} " d " ${gmock_FOUND})

    if(GMOCK_FOUND)
        # If GMock was found in OS, search for GTest too
        find_package(GTest)
    endif()

    if(NOT GTEST_LIBRARIES)
        set(GTEST_LIBRARIES ${GTEST_LIBRARY})
    endif()

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
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

add_library(YAGE INTERFACE)
target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Script YAGE::Core)

configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPY_ONLY)

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()
