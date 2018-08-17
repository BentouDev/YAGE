# Agnes
yage_setup_dependency(Agnes
        CONAN Agnes/0.1@bentoudev/yage
        VAR_NAME AGNES
)

yage_include(YAGE ${AGNES_INCLUDE_DIR})