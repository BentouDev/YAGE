# GLM
if(FALSE)
yage_setup_dependency(GLM
        INCLUDE Dependencies/GLM/Source)

add_library(glm INTERFACE IMPORTED)

target_include_directories(glm
	INTERFACE 
		${GLM_INCLUDE_DIR})

install(DIRECTORY ${GLM_INCLUDE_DIR}/glm DESTINATION include)
endif()

add_library(glm INTERFACE)
target_include_directories(glm
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Dependencies/GLM/Source>)

install(TARGETS glm
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(DIRECTORY ${GLM_INCLUDE_DIR}/glm DESTINATION include)