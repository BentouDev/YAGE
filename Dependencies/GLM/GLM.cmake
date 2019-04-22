# GLM
yage_setup_dependency(GLM
        SOURCE Dependencies/GLM/Source
        INCLUDE Dependencies/GLM/Source)

add_library(glm INTERFACE IMPORTED)

target_include_directories(glm
    INTERFACE 
        ${GLM_INCLUDE_DIR})