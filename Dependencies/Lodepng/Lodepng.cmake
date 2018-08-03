# Lodepng
yage_include(YAGE Dependencies/Lodepng/Source)
add_library(lodepng STATIC Dependencies/Lodepng/Source/lodepng.cpp Dependencies/Lodepng/Source/lodepng.h)

install(TARGETS lodepng
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})