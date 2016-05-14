## Provides various utilities, such as memory management, logger etc.
project(Utils VERSION 0.1 LANGUAGES CXX)

add_library(Utils include/SafeDelete.h src/Debug.cpp include/Container.h include/Color.hpp include/Using.h include/Index.h include/TypeInfo.h include/BorrowedPtr.h src/TypeInfo.cpp)
add_library(Volkhvy::Utils ALIAS Utils)

target_include_directories(Utils PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_compile_features(Utils
        PUBLIC cxx_auto_type cxx_nullptr
        PRIVATE cxx_variadic_templates)

# 'make install' to the correct location
install(TARGETS Utils
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Utils FILE UtilsConfig.cmake)

# Every library has unit tests, of course
add_executable(UtilsTests test/UtilsTests.cpp test/MemoryTests.cpp test/ContainerTests.cpp test/HandleTests.cpp test/UsingTests.cpp test/TypeInfoTests.cpp)

target_link_libraries(UtilsTests
        Utils gtest gtest_main gmock)

add_test(UtilsTests UtilsTests)
add_coverage(Utils)