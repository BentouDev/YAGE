#============================#
# Project setup              #
#============================#
# detect if YAGE is being bundled,
# disable testsuite in that case
if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

# Add modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})
include (Conan)
include (Yage)

# Configure conan
set (YAGE_CONAN_INFO ${EXECUTABLE_OUTPUT_PATH}/conanbuildinfo.cmake)
if (NOT EXISTS YAGE_CONAN_INFO)
    set(YAGE_CONAN_INFO ${CMAKE_CURRENT_SOURCE_DIR}/Build/conanbuildinfo.cmake)
endif()

if(EXISTS YAGE_CONAN_INFO)
    include (YAGE_CONAN_INFO)
    conan_basic_setup()
endif()

# Emit additional metadata for debug builds
include(CMakeToolsHelpers OPTIONAL)

# Pick either lib or lib64
include("GNUInstallDirs")

# Installation
if(NOT LIBDIR)
    set (LIBDIR lib)
endif()

set (YAGE_BINARY_DIR "bin")
set (YAGE_LIBRARY_DIR "${LIBDIR}/YAGE")
set (YAGE_INCLUDE_DIR "include/YAGE")

# Enforce warnings
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif(MSVC)
