# - Find GLFW
# Find the native GLFW includes and libraries
#
#  GLFW_INCLUDE_DIR - where to find GLFW.h, etc.
#  GLFW_LIBRARIES   - List of libraries when using libGLFW.
#  GLFW_FOUND       - True if libGLFW found.

if(GLFW_INCLUDE_DIR)
    # Already in cache, be silent
    set(GLFW_FIND_QUIETLY TRUE)
endif(GLFW_INCLUDE_DIR)

find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h)
find_library(GLFW_LIBRARY NAMES libglfw.so.3.0 libglfw.so.3 libglfw.so.3 glfw3 glfw)

# Handle the QUIETLY and REQUIRED arguments and set GLFW_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR)

if(GLFW_FOUND)
    set(GLFW_LIBRARIES ${GLFW_LIBRARY})
else(GLFW_FOUND)
    set(GLFW_LIBRARIES)
endif(GLFW_FOUND)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)
