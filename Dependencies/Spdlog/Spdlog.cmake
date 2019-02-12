#Spdlog
yage_setup_dependency(Spdlog
        PUBLIC_INSTALL
        VAR_NAME SPDLOG
        NOT_OS
        SOURCE Dependencies/Spdlog/Source
        INCLUDE Dependencies/Spdlog/Source/include)

yage_include(YAGE ${SPDLOG_INCLUDE_DIR})