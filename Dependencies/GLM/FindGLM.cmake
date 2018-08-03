# Find GLM
# Find the native GLM includes
#
# GLM_INCLUDE_DIR - where to find glm/common.hpp, etc.
# GLM_FOUND       - True if libGLM found.

if(GLM_INCLUDE_DIR)
    # Already in cache, be silent
    set(GLM_FIND_QUIETLY TRUE)
endif(GLM_INCLUDE_DIR)

find_path(GLM_INCLUDE_DIR glm/common.hpp)

# Handle the QUIETLY and REQUIRED arguments and set GLFW_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
