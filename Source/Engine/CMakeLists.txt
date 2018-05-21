## Yet Another Game Engine
project(YAGE VERSION 0.1 LANGUAGES CXX C)

add_library(Engine SHARED yage.h)
add_library(YAGE::Engine ALIAS Engine)

add_subdirectory(Utils)
add_subdirectory(Core)
add_subdirectory(Script)

set_target_properties(Engine PROPERTIES
        LINKER_LANGUAGE CXX
        OUTPUT_NAME "YAGE")

target_link_libraries(Engine YAGE::Utils YAGE::Script YAGE::Core)

target_include_directories(Engine PUBLIC
    $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

# 'make install' to the correct location
install(TARGETS Engine
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
