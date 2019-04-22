## Yet Another Game Engine
project(YAGE VERSION 0.1 LANGUAGES CXX C)

add_library(Engine SHARED yage.h yage.cpp)
add_library(YAGE::Engine ALIAS Engine)

add_subdirectory(Kernel)
add_subdirectory(Core)
add_subdirectory(Script)

message("-- yage: Collecting headers from " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE
    YAGE_ALL_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(YAGE_REFLECTION_CONFIG
    ${CMAKE_SOURCE_DIR}/Data/Reflection/engine_main.mustache
    ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_header.mustache
    ${CMAKE_SOURCE_DIR}/Data/Reflection/reflection_unit.mustache
    ${CMAKE_SOURCE_DIR}/Data/Reflection/Yage.json)

yage_add_ctti(Engine
    DIRECTORY Generated/Engine
    PATTERN Data/Reflection/Yage.json
    OUTPUT ${CMAKE_SOURCE_DIR}/Generated/Engine/register_reflection.cpp
    HEADERS ${YAGE_ALL_HEADERS}
    DEPENDS ${YAGE_REFLECTION_CONFIG})

target_link_libraries(Engine_CTTI PUBLIC YAGE::Kernel YAGE::Core)

target_include_directories(Engine_CTTI
    PUBLIC
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

install(TARGETS Engine_CTTI
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Generated/Engine DESTINATION ${YAGE_INCLUDE_DIR}/CTTI/Engine)

target_compile_definitions(Engine PRIVATE ${YAGE_LIB_COMPILE_DEFS})
target_compile_options(Engine PRIVATE ${YAGE_ENFORCE_WARNINGS})

set_target_properties(Engine PROPERTIES
    LINKER_LANGUAGE CXX
    OUTPUT_NAME "YAGE"
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_link_libraries(Engine 
    PUBLIC
        YAGE::Kernel 
        YAGE::Script 
        YAGE::Core 
        CTTI::Engine)

target_include_directories(Engine 
    PUBLIC $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

# 'make install' to the correct location
install(TARGETS Engine
    EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
    ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
    LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
    RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(FILES yage.h DESTINATION ${YAGE_INCLUDE_DIR})
