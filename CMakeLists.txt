## Yet Another Game Engine
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(YAGE VERSION 0.1 LANGUAGES CXX C)

# Project setup
include (${CMAKE_CURRENT_SOURCE_DIR}/Modules/YageSetupProject.cmake)

#============================#
# Version                    #
#============================#

option(YAGE_VERSION_PATCH "Build version" "dev")
option(YAGE_BUILD_TESTS "Build Tests" ${NOT_SUBPROJECT})
option(YAGE_BUILD_EDITOR "Build Editor" ON)
option(YAGE_VALGRIND "Build with valgrind headers" ON)

if (NOT YAGE_VERSION_PATCH)
    set(YAGE_VERSION_PATCH dev)
endif()

set(PROJECT_VERSION_PATCH ${YAGE_VERSION_PATCH})

#============================#
#   _  _   __    ___  ____   #
#  ( \/ ) /__\  / __)( ___)  #
#   \  / /(__)\( (_-. )__)   #
#   (__)(__)(__)\___/(____)  #
#                            #
#   Yet Another Game Engine  #
#                            #
#============================#

message ("-- yage: Version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Define engine library
add_library(YAGE INTERFACE)

# Add Vulkan
find_package(Vulkan)

# Add OpenGl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)

# Add pThreads
find_package(Threads)

# Add Agnes
yage_add_dependency(Dependencies/Agnes)

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
enable_testing()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(YAGE_LIB_COMPILE_DEFS -DYAGE_EXPORTS)
set(YAGE_EXE_COMPILE_DEFS -DYAGE_EDITOR)

# Include all engine modules
add_subdirectory(Source/Engine)

target_link_libraries(YAGE INTERFACE YAGE::Utils YAGE::Script YAGE::Core)
set_target_properties(YAGE PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

# Export yage-config.cmake, so engine user may use find_package
install(EXPORT Engine
        FILE yage-config.cmake
        NAMESPACE Yage::
        DESTINATION "${YAGE_LIBRARY_DIR}")

#============================#
# Editor                     #
#============================#
if(YAGE_BUILD_EDITOR)

    # Add QT5 when building Editor
    find_package(Qt5Widgets CONFIG REQUIRED Qml Quick QuickWidgets)
    find_package(Qt5Qml)
    find_package(Qt5Quick)
    find_package(Qt5QuickWidgets)

    # Additional windows specific features
    if(WIN32)
        yage_add_dependency(Dependencies/WinDeployQt)
        find_package(Qt5WinExtras)
    endif(WIN32)

    # Add AdvancedDockingSystem
    yage_add_dependency(Dependencies/AdvancedDockingSystem)

    # Add Mustache
    yage_add_dependency(Dependencies/Mustache)

    # Include Editor
    add_subdirectory(Source/ProjectManager)
    add_subdirectory(Source/Editor)

endif(YAGE_BUILD_EDITOR)

#============================#
# Post Build                 #
#============================#

# Create log output directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../Log)

# Copy configuration
configure_file(Config/Config.json ${CMAKE_BINARY_DIR} COPYONLY)

# Copy ./Data on install
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Data/ DESTINATION ${YAGE_DATA_DIR})

# Evaluate test coverage when testing is enabled
if(YAGE_BUILD_TESTS)
    coverage_evaluate()
endif()

#============================#
# Post Install               #
#============================#

add_subdirectory(Modules/PostInstall)

#============================#
# Packaging                  #
#============================#
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_VERSION_NAME "YAGE")
include(CPack)