# Json
yage_setup_dependency(Json 
        NOT_OS TRUE
        CONAN jsonformoderncpp/3.1.2@vthiery/stable
        TARGET json
        VAR_NAME JSON)

yage_include(YAGE ${JSON_INCLUDE_DIR})