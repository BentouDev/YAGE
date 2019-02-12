# Find Agnes
# Find Agnes executable and headers

find_program (
    AGNES_BINARY
    NAMES Agnes Agnes.exe agnes agnes.exe
    PATHS ${CONAN_BIN_DIRS_AGNES}
)

find_path (
    AGNES_INCLUDE_DIR
    NAMES Reflection/ReflectionInfo.h Reflection/ReflectionUtils.h
    PATHS ${CONAN_INCLUDE_DIRS_AGNES} /usr/include/ /usr/local/include/
)

# Handle the QUIETLY and REQUIRED arguments and set AGNES_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AGNES DEFAULT_MSG AGNES_BINARY AGNES_INCLUDE_DIR)

mark_as_advanced(AGNES_INCLUDE_DIR AGNES_BINARY)