## Yet Another Game Engine
project(YAGE VERSION 0.1 LANGUAGES CXX C)

add_library(Engine SHARED yage.h yage.cpp)
add_library(YAGE::Engine ALIAS Engine)

add_subdirectory(Utils)
add_subdirectory(Core)
add_subdirectory(Script)

message("-- yage: Collecting headers from " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE
        YAGE_ALL_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/Generated/generated.timestamp ${CMAKE_SOURCE_DIR}/Generated/register_reflection.cpp
        DEPENDS ${YAGE_ALL_HEADERS}
        COMMENT "Generating CTTI..."
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND Agnes ARGS Data/Reflection/Yage.json ${YAGE_ALL_HEADERS}
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_SOURCE_DIR}/Generated/generated.timestamp)

add_custom_target(YAGE_RUN_AGNES 
        DEPENDS ${CMAKE_SOURCE_DIR}/Generated/generated.timestamp
        COMMENT "Checking if CTTI needs regeneration...")

file(GLOB
        YAGE_GENERATED_CRTTI
        ${CMAKE_SOURCE_DIR}/Generated/*.h)

add_library(EngineCRTTI ${YAGE_GENERATED_CRTTI} ${CMAKE_SOURCE_DIR}/Generated/register_reflection.cpp)
add_library(Yage::EngineCRTTI ALIAS EngineCRTTI)
add_dependencies(EngineCRTTI YAGE_RUN_AGNES)
set_target_properties(EngineCRTTI PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)

install(TARGETS EngineCRTTI
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Generated/ DESTINATION ${YAGE_INCLUDE_DIR}/CRTTI)

target_compile_definitions(Engine PRIVATE ${YAGE_LIB_COMPILE_DEFS})

set_target_properties(Engine PROPERTIES
        LINKER_LANGUAGE CXX
        OUTPUT_NAME "YAGE")

target_link_libraries(Engine YAGE::Utils YAGE::Script YAGE::Core Yage::EngineCRTTI)

target_include_directories(Engine 
    PUBLIC $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

# 'make install' to the correct location
install(TARGETS Engine
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
