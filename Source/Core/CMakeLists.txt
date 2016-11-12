## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

set(OPENGL_API Source/Gfx/OpenGl/Generated/gl_core_3_3.hpp Source/Gfx/OpenGl/Generated/gl_core_3_3.cpp Source/Gfx/OpenGl/OpenGLBase.h Source/Gfx/OpenGl/OpenGLBase.cpp Source/Gfx/OpenGl/OpenGLBuffers.h Source/Gfx/OpenGl/OpenGLBuffers.cpp)

if(OPENGL_FOUND)
    set(GRAPHICS_API ${GRAPHICS_API} ${OPENGL_API})
endif()

add_library(Core ${GRAPHICS_API} Source/Resources/Mesh/Mesh.cpp Source/Resources/Mesh/Mesh.h Source/Window.cpp Source/Window.h Source/Platform.h Source/Gfx/Viewport.cpp Source/Gfx/Viewport.h Source/Resources/Resource.h Source/Gfx/RenderTarget.cpp Source/Gfx/RenderTarget.h Source/Engine.cpp Source/Engine.h Source/Gfx/Rectangle.h Source/Config.cpp Source/Config.h Source/Gfx/Renderer.h Source/Logger.h Source/Logic/Scene.cpp Source/Logic/Scene.h Source/Context.h Source/Context.cpp Source/Resources/Material/Material.cpp Source/Resources/Material/Material.h Source/Gfx/Shapes.h Source/Resources/Shader/Shader.cpp Source/Resources/Shader/Shader.h Source/Resources/ResourceFile.h Source/EngineApis.h Source/EngineApis.cpp Source/Logic/RenderingSystem.h Source/Logic/RenderingSystem.cpp Source/Gfx/Shapes.cpp Source/Logic/Component.h Source/Resources/ResourceManager.cpp Source/Resources/ResourceManager.h Source/Gfx/Renderer.cpp Source/Resources/Shader/ShaderBuilder.cpp Source/Resources/Shader/ShaderBuilder.h Source/Resources/Mesh/MeshSchemeManager.h Source/Resources/Mesh/MeshSchemeManager.cpp Source/GameTime.h Source/Gfx/StaticBuffer.h Source/Resources/Mesh/MeshBuilder.cpp Source/Resources/Mesh/MeshBuilder.h Source/Resources/Mesh/MeshManager.h Source/Resources/Mesh/MeshManager.cpp Source/Resources/Material/MaterialManager.h Source/Resources/Material/MaterialManager.cpp Source/Gfx/CommandQueue.h Source/Gfx/StaticBuffer.cpp Source/Gfx/BufferManager.cpp Source/Gfx/BufferManager.h Source/Gfx/Camera.cpp Source/Gfx/Camera.h Source/Logic/RenderingComponent.cpp Source/Logic/RenderingComponent.h Source/Resources/Mesh/MeshScheme.h Source/Resources/Shader/ShaderManager.cpp Source/Resources/Shader/ShaderManager.h)
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
    target_compile_definitions(Core PRIVATE YAGE_OPENGL=1)
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

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(CoreTests
            Tests/CoreTests.cpp Tests/DeviceMock.h Tests/EngineTests.cpp)

    target_link_libraries(CoreTests
            Core glfw gtest gtest_main gmock)

    add_test(CoreTests CoreTests)
    add_coverage(Core)
endif(YAGE_BUILD_TESTS)