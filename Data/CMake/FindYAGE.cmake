# Find YAGE.
#
# This module defines:
#  YAGE_INCLUDE_DIR, where to find yage headers
#  YAGE_EDITOR, the yage editor executable.
#  YAGE_LIBRARY, the yage engine library.
#  YAGE_FOUND, If false, do not try to use yage.

find_path(YAGE_INCLUDE_DIR
        NAMES Core/Engine.h
        HINTS /usr/include /usr/local/include
              /usr/include/yage /usr/local/include/yage
              /usr/include/YAGE /usr/local/include/YAGE)

find_program(YAGE_EDITOR
        NAMES Editor Editor.exe YageEditor YageEditor.exe
        PATH /usr/bin /usr/local/bin ${YAGE_PREFIX}/bin)

find_library(YAGE_LIBRARY
        NAMES libyage.so libYage.so libYAGE.so yage Yage YAGE
        HINTS ${YAGE_PREFIX}/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YAGE DEFAULT_MSG
        YAGE_INCLUDE_DIR
        YAGE_PROGRAM
        YAGE_LIBRARY)

mark_as_advanced(YAGE_INCLUDE_DIR YAGE_PROGRAM YAGE_LIBRARY)
