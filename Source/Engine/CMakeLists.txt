## Yet Another Game Engine
project(YAGE VERSION 0.1 LANGUAGES CXX C)

add_library(Engine INTERFACE)
add_library(YAGE::Engine ALIAS Engine)

add_subdirectory(Utils)
add_subdirectory(Core)
add_subdirectory(Script)

target_link_libraries(Engine INTERFACE YAGE::Utils YAGE::Script YAGE::Core)
