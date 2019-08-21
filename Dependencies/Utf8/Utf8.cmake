# Utf8
add_library(utf8 INTERFACE)
target_include_directories(utf8
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Utf8/Source/source>)

install(TARGETS utf8
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})