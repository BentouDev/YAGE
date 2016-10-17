## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

set(OPENGL_API Source/Gfx/OpenGl/Generated/gl_core_3_3.hpp Source/Gfx/OpenGl/Generated/gl_core_3_3.cpp Source/Gfx/OpenGl/OpenGLBase.h Source/Gfx/OpenGl/OpenGLBase.cpp Source/Gfx/OpenGl/OpenGLBuffers.h Source/Gfx/OpenGl/MeshApi.cpp Source/Gfx/OpenGl/MeshApi.h Source/Gfx/OpenGl/OpenGLBatch.cpp Source/Gfx/OpenGl/OpenGLBatch.h Source/Gfx/OpenGl/OpenGLBuffers.cpp)

if(OPENGL_FOUND)
    set(GRAPHICS_API ${GRAPHICS_API} ${OPENGL_API} Source/Logic/Component.h)
endif()

add_library(Core ${GRAPHICS_API} Source/Resources/Mesh/Mesh.cpp Source/Resources/Mesh/Mesh.h Source/Window.cpp Source/Window.h Source/Platform.h Source/Gfx/Viewport.cpp Source/Gfx/Viewport.h Source/Resources/Resource.h Source/Resources/ResourceManager.h Source/Gfx/RenderTarget.cpp Source/Gfx/RenderTarget.h Source/Engine.cpp Source/Engine.h Source/Gfx/Rectangle.h Source/Config.cpp Source/Config.h Source/Gfx/Renderer.h Source/Logger.h Source/Resources/Mesh/MeshFactory.cpp Source/Resources/Mesh/MeshFactory.h Source/Logic/Scene.cpp Source/Logic/Scene.h Source/Context.h Source/Context.cpp Source/Resources/Material/Material.cpp Source/Resources/Material/Material.h Source/Gfx/Buffer.cpp Source/Gfx/Buffer.h Source/Gfx/Shapes.h Source/Resources/Shader/Shader.cpp Source/Resources/Shader/Shader.h Source/Resources/Shader/ShaderFactory.h Source/Resources/Shader/ShaderFactory.cpp Source/Resources/ResourceFile.h Source/Resources/Shader/ShaderManager.h Source/Resources/Mesh/MeshManager.cpp Source/Resources/Mesh/MeshManager.h Source/EngineApis.h Source/EngineApis.cpp Source/Gfx/RenderBucket.h Source/Logic/RenderingSystem.h Source/Logic/RenderingSystem.cpp Source/Gfx/Shapes.cpp Source/Gfx/RenderBucket.cpp)
add_library(YAGE::Core ALIAS Core)

target_include_directories(Core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source )

target_compile_features(Core
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        cxx_nullptr)

target_link_libraries(Core YAGE::Utils glm glfw)

if(OPENGL_FOUND)
    target_compile_definitions(Core PRIVATE VOLKHVY_OPENGL=1)
    target_include_directories(Core PRIVATE ${OPENGL_INCLUDE_DIR})
    target_link_libraries(Core ${OPENGL_LIBRARIES})
endif()

# 'make install' to the correct location
install(TARGETS Core
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the build directory
export(TARGETS glfw Core FILE CoreConfig.cmake)

# Every library has unit tests, of course
add_executable(CoreTests
        Tests/CoreTests.cpp Tests/DeviceMock.h Tests/EngineTests.cpp)

target_link_libraries(CoreTests
        Core glfw gtest gtest_main gmock)

add_test(CoreTests CoreTests)
add_coverage(Core)