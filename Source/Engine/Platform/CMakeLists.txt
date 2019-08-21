## Provides implementation of low level platform dependent API's
project (Platform VERSION 0.1 LANGUAGES CXX)

yage_glob_files(
    PLATFORM_HEADERS 
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/include/*.h
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hxx)

yage_glob_files(
    PLATFORM_SOURCES 
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cxx)

add_library(Platform SHARED ${PLATFORM_HEADERS} ${PLATFORM_SOURCES})
add_library(Yage::Platform ALIAS Platform)

target_include_directories(Platform 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>
    PRIVATE
        src)

target_compile_features(Platform
    PUBLIC
        cxx_auto_type
        cxx_variadic_templates
        cxx_nullptr
        cxx_std_17)

set_target_properties(Platform PROPERTIES
    LINKER_LANGUAGE CXX
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_compile_definitions(Platform PRIVATE ${YAGE_LIB_COMPILE_DEFS})
target_compile_options(Platform PRIVATE ${YAGE_ENFORCE_WARNINGS})

target_link_libraries(Platform
    PUBLIC
        Yage::Kernel
        Yage::DEP::eastl
        # ToDo: Create custom keycodes and remove glfw from public namespace
        Yage::DEP::glfw
        glm
    PRIVATE
        Yage::DEP::sdl2
        Yage::DEP::jsonformoderncpp
        ${CMAKE_THREAD_LIBS_INIT})

# 'make install' to the correct location
install(TARGETS Platform ${OPENGL_GEN_TARGET}
    EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
    ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
    LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
    RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(DIRECTORY include/ DESTINATION ${YAGE_INCLUDE_DIR})

# This makes the project importable from the build directory
export(TARGETS Platform ${OPENGL_GEN_TARGET} FILE PlatformConfig.cmake)
