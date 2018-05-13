# Find GoogleMock
# Find the native GoogleMock includes and libraries
#
# GMOCK_INCLUDE_DIR - where to find gmock/gmock.h, etc.
# GMOCK_LIBRARIES   - List of libraries when using libgtest.
# GMOCK_FOUND       - True if libgmock found.

if(GMOCK_INCLUDE_DIR)
    # Already in cache, be silent
    set(GMOCK_FIND_QUIETLY TRUE)
endif(GMOCK_INCLUDE_DIR)

find_path(GMOCK_INCLUDE_DIR gmock/gmock.h)
find_library(GMOCK_LIBRARY NAMES libgmock.so libgmock.so.0 libgmock.so.0.0 gmock)

# Handle the QUIETLY and REQUIRED arguments and set GTEST_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR)

if(GMOCK_FOUND)
    set(GMOCK_LIBRARIES ${GMOCK_LIBRARY})
else()
    set(GMOCK_LIBRARIES)
endif()

mark_as_advanced(GMOCK_INCLUDE_DIR GMOCK_LIBRARY)
