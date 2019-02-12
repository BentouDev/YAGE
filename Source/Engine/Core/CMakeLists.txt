## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE
    CORE_HEADERS
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES false
    ${CMAKE_CURRENT_LIST_DIR}/include/*.h
    ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)

file(GLOB_RECURSE
    CORE_SOURCES
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES false
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cxx)

add_library(Core STATIC ${GRAPHICS_API} ${CORE_HEADERS} ${CORE_SOURCES})
add_library(YAGE::Core ALIAS Core)

target_include_directories(Core 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Kernel/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>
    PRIVATE
        src)

#set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 17)
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
        YAGE::Kernel
        ${GLFW_LIBRARIES}
    PRIVATE
        lodepng
        utf8
        ${ASSIMP_LIBRARIES}
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
export(TARGETS ${GLFW_TARGET} ${ASSIMP_TARGETS} lodepng utf8 Core FILE CoreConfig.cmake)

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(CoreTests
        tests/CoreTests.cpp tests/MemoryModuleTests.cpp tests/WorldTests.cpp)

    target_link_libraries(CoreTests
        Core ${GMOCK_LIBRARY} ${GTEST_LIBRARIES} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    add_test(CoreTests CoreTests)
    add_coverage(Core)
endif(YAGE_BUILD_TESTS)