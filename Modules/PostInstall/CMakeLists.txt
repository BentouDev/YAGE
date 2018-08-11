if (UNIX)
    # Workaround to run ldconfig on linux after install of shared library
    install(CODE "execute_process(COMMAND export LD_RUN_PATH=/usr/local/lib)")
    install(CODE "execute_process(COMMAND export LD_RUN_PATH=${YAGE_LIBRARY_DIR})")
    install(CODE "execute_process(COMMAND ldconfig)")
endif()