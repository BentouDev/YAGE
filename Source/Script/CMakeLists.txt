## Provides easy to use bindings for lua and squirell
project(Script VERSION 0.1 LANGUAGES CXX)

add_library(Script Source/Binding.cpp Source/Binding.h)
add_library(YAGE::Script ALIAS Script)

target_include_directories(Script PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source)

target_compile_features(Script
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates)

# 'make install' to the correct location
install(TARGETS Script
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Script FILE ScriptConfig.cmake)

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(ScriptTests
            Tests/BindingTest.cpp)

    target_link_libraries(ScriptTests
            Script gtest gtest_main)

    add_test(ScriptTests ScriptTests)
    add_coverage(Script)
endif(YAGE_BUILD_TESTS)
