## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

message("-- yage: Collecting headers from " ${YAGE_SOURCE_DIR})

# Engine CTTI
yage_glob_files(
    YAGE_ALL_HEADERS
    PATTERNS
        ${YAGE_ENGINE_SOURCE_DIR}/*.h
        ${YAGE_ENGINE_SOURCE_DIR}/*.hpp
        ${YAGE_ENGINE_SOURCE_DIR}/*.hxx)

set(YAGE_REFLECTION_CONFIG
    ${YAGE_SOURCE_DIR}/Data/Reflection/engine_main.mustache
    ${YAGE_SOURCE_DIR}/Data/Reflection/reflection_header.mustache
    ${YAGE_SOURCE_DIR}/Data/Reflection/reflection_unit.mustache
    ${YAGE_SOURCE_DIR}/Data/Reflection/Yage.json)

yage_add_ctti(Engine
    DIRECTORY ${CMAKE_SOURCE_DIR}/out/gen/CTTI/Core
    PATTERN ${YAGE_SOURCE_DIR}/Data/Reflection/Yage.json
    OUTPUT ${CMAKE_SOURCE_DIR}/out/gen/CTTI/Core/register_reflection.cpp
    HEADERS ${YAGE_ALL_HEADERS}
    DEPENDS ${YAGE_REFLECTION_CONFIG})

target_link_libraries(Engine_CTTI PUBLIC Yage::Kernel Yage::Platform Yage::Gfx)

target_include_directories(Engine_CTTI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/out/gen>
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>)

install(TARGETS Engine_CTTI
        EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/out/gen/CTTI DESTINATION ${YAGE_INCLUDE_DIR}/)

# Core
yage_glob_files(
    CORE_HEADERS
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/include/*.h
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hxx)

yage_glob_files(
    CORE_SOURCES
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cxx)

add_library(Core SHARED ${GRAPHICS_API} ${CORE_HEADERS} ${CORE_SOURCES} $<TARGET_OBJECTS:Engine_CTTI>)
add_library(Yage::Core ALIAS Core)

target_include_directories(Core 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/out/gen>
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>
    PRIVATE
        src)

target_compile_features(Core
    PUBLIC
        cxx_auto_type
        cxx_variadic_templates
        cxx_nullptr
        cxx_std_17)

set_target_properties(Core PROPERTIES
    LINKER_LANGUAGE CXX
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_compile_definitions(Core PRIVATE ${YAGE_LIB_COMPILE_DEFS})
target_compile_options(Core PRIVATE ${YAGE_ENFORCE_WARNINGS})

target_link_libraries(Core
    PUBLIC
        Yage::Platform
        Yage::Gfx
        Yage::Kernel
    PRIVATE
        lodepng
        utf8
        Yage::DEP::assimp
        Yage::DEP::jsonformoderncpp
        ${CMAKE_THREAD_LIBS_INIT})

if(OPENGL_FOUND)
    target_compile_definitions(Core PRIVATE YAGE_OPENGL=1)
    target_include_directories(Core PRIVATE ${OPENGL_INCLUDE_DIR})
    target_link_libraries(Core PRIVATE ${OPENGL_LIBRARIES})
endif()

# 'make install' to the correct location
install(TARGETS Core
    EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
    ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
    LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
    RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(DIRECTORY include/ DESTINATION ${YAGE_INCLUDE_DIR})

# This makes the project importable from the build directory
export(TARGETS Engine_CTTI lodepng glm utf8 Core FILE CoreConfig.cmake)

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(CoreTests
        tests/CoreTests.cpp tests/MemoryModuleTests.cpp tests/WorldTests.cpp)

    target_link_libraries(CoreTests
        Core ${GMOCK_LIBRARY} ${GTEST_LIBRARIES} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    add_test(CoreTests CoreTests)
    add_coverage(Core)
endif(YAGE_BUILD_TESTS)