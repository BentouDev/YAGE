#Mustache
yage_setup_dependency(Mustache
        NOT_OS TRUE
        VAR_NAME MUSTACHE
        SOURCE Dependencies/Mustache/Source
        INCLUDE Dependencies/Mustache/Source)

yage_include(YAGE ${MUSTACHE_INCLUDE_DIR})