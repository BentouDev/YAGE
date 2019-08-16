## Provides various utilities, such as memory management, logger etc.
project(Kernel VERSION 0.1 LANGUAGES CXX)

yage_glob_files(
    KERNEL_HEADERS
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/include/*.h
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hxx)

yage_glob_files(
    KERNEL_SOURCES
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/src/*.c
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cxx)

add_library(Kernel SHARED ${KERNEL_SOURCES})
add_library(Yage::Kernel ALIAS Kernel)

target_include_directories(Kernel PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>
    PRIVATE src ${CMAKE_CURRENT_BINARY_DIR})

target_link_options(Kernel PUBLIC 
    $<BUILD_INTERFACE:/NATVIS:${eastl_LIB_DIRS}/EASTL.natvis>
    $<INSTALL_INTERFACE:/NATVIS:\${_IMPORT_PREFIX}/${YAGE_LIBRARY_DIR}/EASTL.natvis>)

target_compile_features(Kernel 
    PUBLIC
        cxx_nullptr
        cxx_auto_type
        cxx_variadic_templates
        cxx_trailing_return_types
        cxx_return_type_deduction
        cxx_right_angle_brackets
        cxx_constexpr
        cxx_std_17)

set_target_properties(Kernel PROPERTIES
    LINKER_LANGUAGE CXX
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_compile_definitions(Kernel PRIVATE ${YAGE_LIB_COMPILE_DEFS})
target_compile_options(Kernel PRIVATE ${YAGE_ENFORCE_WARNINGS})

target_link_libraries(Kernel
    PUBLIC
        Yage::DEP::eastl
        Yage::DEP::spdlog
        Yage::DEP::fmt)

if(VALGRIND_FOUND)
    target_compile_definitions(Kernel PRIVATE YAGE_VALGRIND=1)
    target_include_directories(Kernel PRIVATE ${VALGRIND_INCLUDE_DIR})
endif()

# 'make install' to the correct location
install(TARGETS Kernel
    EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
    ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
    LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
    RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(DIRECTORY include/ DESTINATION ${YAGE_INCLUDE_DIR})

# This makes the project importable from the build directory
export(TARGETS Kernel FILE KernelConfig.cmake)

if(YAGE_BUILD_TESTS)
    file (GLOB
        KERNEL_TESTS_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/test/*.c
        ${CMAKE_CURRENT_LIST_DIR}/test/*.cpp)

    # Every library has unit tests, of course
    add_executable(KernelTests ${KERNEL_TESTS_SOURCES})

    target_link_libraries(KernelTests
            Kernel ${CMAKE_THREAD_LIBS_INIT})

    add_test(KernelTests KernelTests)
    add_coverage(Kernel)
endif(YAGE_BUILD_TESTS)