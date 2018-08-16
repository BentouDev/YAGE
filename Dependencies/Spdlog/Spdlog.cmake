#Spdlog
yage_setup_dependency(Spdlog
        NOT_OS TRUE
        CONAN spdlog/1.0.0@bincrafters/stable
        VAR_NAME SPDLOG)

yage_include(YAGE ${SPDLOG_INCLUDE_DIR})