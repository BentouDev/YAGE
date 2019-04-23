# GLM
yage_setup_dependency(GLM
        INCLUDE Dependencies/GLM/Source)

add_library(glm INTERFACE IMPORTED)

target_include_directories(glm
	INTERFACE 
		${GLM_INCLUDE_DIR})