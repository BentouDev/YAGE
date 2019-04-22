# Lodepng
add_library(lodepng STATIC Dependencies/Lodepng/Source/lodepng.cpp Dependencies/Lodepng/Source/lodepng.h)
target_include_directories(lodepng PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Lodepng/Source>)

install(TARGETS lodepng
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})