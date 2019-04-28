## Provides easy to use bindings for lua and squirell
project(Script VERSION 0.1 LANGUAGES CXX)

add_library(Script STATIC Source/Binding.cpp Source/Binding.h)
add_library(Yage::Script ALIAS Script)

target_include_directories(Script PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE Source)

target_compile_features(Script
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates)

# 'make install' to the correct location
install(TARGETS Script
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
        ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
        LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
        RUNTIME  DESTINATION ${YAGE_BINARY_DIR})

# This makes the project importable from the build directory
export(TARGETS Script FILE ScriptConfig.cmake)

# Every library has unit tests, of course
#add_executable(ScriptTests
        #Tests/BindingTest.cpp)

#target_link_libraries(ScriptTests
        #Script gtest gtest_main)

#add_test(ScriptTests ScriptTests)
#add_coverage(Script)