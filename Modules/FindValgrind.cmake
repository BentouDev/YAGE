# Find Valgrind.
#
# This module defines:
#  VALGRIND_INCLUDE_DIR, where to find valgrind headers
#  VALGRIND_PROGRAM, the valgrind executable.
#  VALGRIND_FOUND, If false, do not try to use valgrind.

find_path(VALGRIND_INCLUDE_DIR memcheck.h
        /usr/include /usr/include/valgrind /usr/local/include /usr/local/include/valgrind)
find_program(VALGRIND_PROGRAM NAMES valgrind PATH /usr/bin /usr/local/bin ${VALGRIND_PREFIX}/bin)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VALGRIND DEFAULT_MSG
        VALGRIND_INCLUDE_DIR
        VALGRIND_PROGRAM)

mark_as_advanced(VALGRIND_INCLUDE_DIR VALGRIND_PROGRAM)
