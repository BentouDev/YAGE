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


# Emit additional metadata for debug builds
include(CMakeToolsHelpers OPTIONAL)

# Pick either lib or lib64
include("GNUInstallDirs")

# Installation
set(LIBDIR ${CMAKE_INSTALL_LIB})
if(NOT LIBDIR)
    set (LIBDIR lib)
endif()

# set(SHAREDSTATEDIR ${CMAKE_INSTALL_SHAREDSTATEDIR})
if(NOT SHAREDSTATEDIR)
    if(WIN32)
        set (SHAREDSTATEDIR "bin/Data")
    elseif(UNIX)
        set (SHAREDSTATEDIR "share/YAGE")
    endif()
endif()

message ("-- yage: install data dir " ${SHAREDSTATEDIR})

set (YAGE_BINARY_DIR "bin")
set (YAGE_LIBRARY_DIR "${LIBDIR}/YAGE")
set (YAGE_INCLUDE_DIR "include/YAGE")
set (YAGE_DATA_DIR ${SHAREDSTATEDIR})

set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Enforce warnings
if(CMAKE_COMPILER_IS_GNUCC)
    #set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wno-reorder")
    set (YAGE_ENFORCE_WARNINGS -Wall -Wno-reorder)
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
    # set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4 /wd4201 ")
    set (YAGE_ENFORCE_WARNINGS /W4 /wd4201)
endif(MSVC)
