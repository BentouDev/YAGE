## Provides basic obj-like file parser
project(Parser VERSION 0.1 LANGUAGES CXX)

add_library(Parser Source/BaseParser.cpp Source/BaseParser.h)
add_library(Volkhvy::Parser ALIAS Parser)

target_include_directories(Parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source)

target_compile_features(Parser
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates)

# 'make install' to the correct location
install(TARGETS Parser
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Parser FILE ParserConfig.cmake)

# Every library has unit tests, of course
add_executable(ParserTests
        Tests/ParserTests.cpp)

target_link_libraries(ParserTests
        Parser gtest gtest_main)

add_test(ParserTests ParserTests)