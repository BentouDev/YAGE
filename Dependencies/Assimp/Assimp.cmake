# Assimp
yage_setup_dependency(Assimp
        CONAN Assimp/0.1@bentoudev/yage
        VAR_NAME ASSIMP
        TARGET assimp)

yage_include(YAGE ${ASSIMP_INCLUDE_DIR})

# Windows needs zlibstatic and IrrXml in assimp export target
if((NOT ${ASSIMP_FOUND}) AND (WIN32))
    if(MINGW)
        set(ASSIMP_TARGETS assimp zlibstatic)
    else()
        set(ASSIMP_TARGETS assimp zlibstatic)
    endif()
else()
    set(ASSIMP_TARGETS ${ASSIMP_TARGET})
endif()