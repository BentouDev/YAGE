## Provides various utilities, such as memory management, logger etc.
project(Utils VERSION 0.1 LANGUAGES CXX)

if(VALGRIND_FOUND)
    set(VALGRIND_ADDITIONAL_HEADERS ${VALGRIND_INCLUDE_DIR} include/Utils/ValgrindMemoryBoundChecker.h include/Utils/ValgrindMemoryTracker.h)
endif(VALGRIND_FOUND)

add_library(Utils ${VALGRIND_ADDITIONAL_HEADERS} include/Utils/SafeDelete.h src/Debug.cpp include/Utils/Handle.h include/Utils/Container.h include/Utils/Color.hpp include/Utils/Using.h include/Utils/Index.h include/Utils/TypeInfo.h include/Utils/BorrowedPtr.h src/TypeInfo.cpp include/Utils/DefaultTrait.h include/Utils/File.h include/Utils/Allocator.h src/LinearAllocator.cpp include/Utils/LinearAllocator.h include/Utils/DebugSourceInfo.h include/Utils/MemoryBlock.h src/StackAllocator.cpp include/Utils/StackAllocator.h src/FreeListAllocator.cpp include/Utils/FreeListAllocator.h src/PoolAllocator.cpp include/Utils/PoolAllocator.h include/Utils/List.h include/Utils/String.h src/String.cpp include/Utils/Dictionary.h include/Utils/PropertyMacro.h include/Utils/MemoryBoundChecker.h include/Utils/MemoryTracker.h include/Utils/MemorySizes.h)
add_library(YAGE::Utils ALIAS Utils)

target_include_directories(Utils PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_compile_features(Utils
        PUBLIC cxx_auto_type cxx_nullptr cxx_variadic_templates)

if(VALGRIND_FOUND)
    target_compile_definitions(Utils PRIVATE YAGE_VALGRIND=1)
    target_include_directories(Utils PRIVATE ${VALGRIND_INCLUDE_DIR})
endif()

# 'make install' to the correct location
install(TARGETS Utils
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS Utils FILE UtilsConfig.cmake)

# Every library has unit tests, of course
add_executable(UtilsTests test/UtilsTests.cpp test/MemoryTests.cpp test/ContainerTests.cpp test/HandleTests.cpp test/UsingTests.cpp test/TypeInfoTests.cpp test/LinearAllocatorTest.cpp test/StackAllocatorTest.cpp test/PoolAllocatorTest.cpp test/FreeListAllocatorTest.cpp test/ListTests.cpp test/StringTests.cpp test/DictionaryTests.cpp)

target_link_libraries(UtilsTests
        Utils gtest gtest_main gmock)

add_test(UtilsTests UtilsTests)
add_coverage(Utils)