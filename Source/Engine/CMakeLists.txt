## Yet Another Game Engine
project(YAGE VERSION 0.1 LANGUAGES CXX C)

add_library(Engine INTERFACE)
add_library(YAGE::Engine ALIAS Engine)

add_subdirectory(Utils)
add_subdirectory(Core)
add_subdirectory(Script)

target_link_libraries(Engine INTERFACE YAGE::Utils YAGE::Script YAGE::Core)

# 'make install' to the correct location
install(TARGETS Engine
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
#install(DIRECTORY include/ DESTINATION include)
