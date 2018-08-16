#Spdlog
yage_setup_dependency(Spdlog
        NOT_OS TRUE
        VAR_NAME SPDLOG
        SOURCE Dependencies/Spdlog/Source
        INCLUDE Dependencies/Spdlog/Source/include)

yage_include(YAGE ${SPDLOG_INCLUDE_DIR})