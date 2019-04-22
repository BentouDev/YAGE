#Spdlog
yage_setup_dependency(Spdlog
        PUBLIC_INSTALL
		CONAN spdlog/1.3.1@bincrafters/stable 
        VAR_NAME SPDLOG
        NOT_OS TRUE)