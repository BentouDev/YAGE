# Agnes
yage_setup_dependency(Agnes
        CONAN Agnes/0.3@bentoudev/yage
        OS_AFTER_CONAN True
        VAR_NAME AGNES
)

yage_include(YAGE ${AGNES_INCLUDE_DIR})