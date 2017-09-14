# Find GoogleTest
# Find the native GoogleTest includes and libraries
#
# GTEST_INCLUDE_DIR - where to find gtest/gtest.h, etc.
# GTEST_LIBRARIES   - List of libraries when using libgtest.
# GTEST_FOUND       - True if libgtest found.

if(GTEST_INCLUDE_DIR)
    # Already in cache, be silent
    set(GTEST_FIND_QUIETLY TRUE)
endif(GTEST_INCLUDE_DIR)

find_path(GTEST_INCLUDE_DIR gtest/gtest.h)
find_library(GTEST_LIBRARY NAMES libgtest.so gtest)

# Handle the QUIETLY and REQUIRED arguments and set GTEST_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTEST DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR)

if(GTEST_FOUND)
    set(GTEST_LIBRARIES ${GTEST_LIBRARY})
else()
    set(GTEST_LIBRARIES)
endif()

mark_as_advanced(GTEST_INCLUDE_DIR GTEST_LIBRARY)
