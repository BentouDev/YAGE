## Graphics - rendering, command buffers, batching and all that stuff
project (Gfx VERSION 0.1 LANGUAGES CXX)

yage_glob_files(
    GFX_HEADERS 
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/include/*.h
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/include/*.hxx)

yage_glob_files(
    GFX_SOURCES 
    PATTERNS
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/*.cxx)

add_library(Gfx SHARED ${GFX_HEADERS} ${GFX_SOURCES})
add_library(Yage::Gfx ALIAS Gfx)

target_include_directories(Gfx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${YAGE_INCLUDE_DIR}>
    PRIVATE
        src)

target_compile_features(Gfx
    PUBLIC
        cxx_auto_type
        cxx_variadic_templates
        cxx_nullptr
        cxx_std_17)

set_target_properties(Gfx PROPERTIES
    LINKER_LANGUAGE CXX
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

target_compile_definitions(Gfx PRIVATE ${YAGE_LIB_COMPILE_DEFS})
target_compile_options(Gfx PRIVATE ${YAGE_ENFORCE_WARNINGS})

target_link_libraries(Gfx
    PUBLIC
        Yage::Kernel
        Yage::Platform
        Yage::DEP::eastl
        glm
    PRIVATE
        Yage::DEP::jsonformoderncpp
        ${CMAKE_THREAD_LIBS_INIT})

if(OPENGL_FOUND)
    add_library(OpenGLGen gen/Gfx/Graphics/OpenGL/gl_core_3_3.cpp)
    target_include_directories(OpenGLGen
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gen>)

    target_compile_definitions(Gfx PRIVATE YAGE_OPENGL=1)
    target_include_directories(Gfx PRIVATE ${OPENGL_INCLUDE_DIR})
    target_link_libraries(Gfx PUBLIC OpenGLGen PRIVATE ${OPENGL_LIBRARIES})

    # Copy generated files on install
    install(DIRECTORY gen/ DESTINATION ${YAGE_INCLUDE_DIR})

    set(OPENGL_GEN_TARGET OpenGLGen)
endif()

# 'make install' to the correct location
install(TARGETS Gfx ${OPENGL_GEN_TARGET}
    EXPORT Engine DESTINATION "${YAGE_LIBRARY_DIR}"
    ARCHIVE  DESTINATION ${YAGE_LIBRARY_DIR}
    LIBRARY  DESTINATION ${YAGE_LIBRARY_DIR}
    RUNTIME  DESTINATION ${YAGE_BINARY_DIR})
install(DIRECTORY include/ DESTINATION ${YAGE_INCLUDE_DIR})

# This makes the project importable from the build directory
export(TARGETS Gfx ${OPENGL_GEN_TARGET} FILE GfxConfig.cmake)
