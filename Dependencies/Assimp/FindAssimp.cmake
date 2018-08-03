# Find Assimp
# Find the native Assimp includes and libraries
#
# ASSIMP_INCLUDE_DIR - where to find headers
# ASSIMP_LIBRARIES   - List of libraries when using libAssimp.
# ASSIMP_FOUND       - True if libAssimp found.

if(ASSIMP_INCLUDE_DIR)
    # Already in cache, be silent
    set(ASSIMP_FIND_QUIETLY TRUE)
endif(ASSIMP_INCLUDE_DIR)

find_path(ASSIMP_INCLUDE_DIR
        NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
        PATHS /usr/include/ /usr/local/include/)
find_library(ASSIMP_LIBRARY NAMES libassimp.so.4.0.1 libassimp.so.4 libassimp.so assimp)

# Handle the QUIETLY and REQUIRED arguments and set GLFW_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)

if(ASSIMP_FOUND)
    set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
else()
    set(ASSIMP_LIBRARIES)
endif()

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)