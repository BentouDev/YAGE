# Find Spdlog
# Find the native Spdlog includes
#
# SPDLOG_INCLUDE_DIR - where to find glm/common.hpp, etc.
# SPDLOG_FOUND       - True if libSpdlog found.

if(SPDLOG_INCLUDE_DIR)
    # Already in cache, be silent
    set(SPDLOG_FIND_QUIETLY TRUE)
endif(SPDLOG_INCLUDE_DIR)

find_path(SPDLOG_INCLUDE_DIR glm/common.hpp)

# Handle the QUIETLY and REQUIRED arguments and set GLFW_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SPDLOG DEFAULT_MSG SPDLOG_INCLUDE_DIR)

mark_as_advanced(SPDLOG_INCLUDE_DIR)
