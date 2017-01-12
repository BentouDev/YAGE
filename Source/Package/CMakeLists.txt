## YAGE Package file format library
project(Package VERSION 0.1 LANGUAGES CXX)

add_library(Package include/Package/Package.h include/Package/VirtualFile.h src/VirtualFile.cpp include/Package/PropertyRecord.h include/Package/VirtualCatalog.h include/Package/RecordDictionary.h include/Package/Token.h include/Package/VirtualRecord.h src/VirtualRecord.cpp)
add_library(YAGE::Package ALIAS Package)

target_include_directories(Package PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_compile_features(Package PUBLIC
        cxx_nullptr
        cxx_auto_type
        cxx_variadic_templates
        cxx_trailing_return_types
        cxx_return_type_deduction
        cxx_right_angle_brackets
        cxx_constexpr)

target_link_libraries(Package YAGE::Utils stdc++fs)

# 'make install' to the correct location
install(TARGETS Package
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Package FILE PackageConfig.cmake)

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(PackageTests test/PackageTests.cpp)

    target_link_libraries(PackageTests
            Package gtest gtest_main gmock)

    add_test(PackageTests PackageTests)
    add_coverage(Package)
endif(YAGE_BUILD_TESTS)
