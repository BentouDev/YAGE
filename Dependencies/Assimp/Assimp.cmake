# Assimp
yage_setup_dependency(Assimp
        CONAN assimp/Release3.1_RC1-dev@bentoudev/stable
        VAR_NAME ASSIMP
        TARGET assimp)

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