## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

set(OPENGL_API include/Core/Gfx/OpenGl/Generated/gl_core_3_3.hpp src/Gfx/OpenGl/Generated/gl_core_3_3.cpp include/Core/Gfx/OpenGl/OpenGLBase.h src/Gfx/OpenGl/OpenGLBase.cpp include/Core/Gfx/OpenGl/OpenGLBuffers.h src/Gfx/OpenGl/OpenGLBuffers.cpp)

if(OPENGL_FOUND)
    set(GRAPHICS_API ${GRAPHICS_API} ${OPENGL_API})
endif()

add_library(Core ${GRAPHICS_API} src/Resources/Mesh/Mesh.cpp include/Core/Resources/Mesh/Mesh.h src/Window.cpp include/Core/Window.h include/Core/Platform.h src/Gfx/Viewport.cpp include/Core/Gfx/Viewport.h include/Core/Resources/Resource.h src/Gfx/RenderTarget.cpp include/Core/Gfx/RenderTarget.h src/Engine.cpp include/Core/Engine.h include/Core/Gfx/Rectangle.h src/Config.cpp include/Core/Config.h include/Core/Gfx/Renderer.h include/Core/Logger.h src/Logic/Scene.cpp include/Core/Logic/Scene.h include/Core/Context.h src/Context.cpp src/Resources/Material/Material.cpp include/Core/Resources/Material/Material.h include/Core/Gfx/Shapes.h src/Resources/Shader/Shader.cpp include/Core/Resources/Shader/Shader.h include/Core/Resources/ResourceFile.h include/Core/EngineApis.h src/EngineApis.cpp include/Core/Logic/RenderingSystem.h src/Logic/RenderingSystem.cpp src/Gfx/Shapes.cpp include/Core/Logic/Component.h src/Resources/ResourceManager.cpp include/Core/Resources/ResourceManager.h src/Gfx/Renderer.cpp src/Resources/Shader/ShaderBuilder.cpp include/Core/Resources/Shader/ShaderBuilder.h include/Core/Resources/Mesh/MeshSchemeManager.h src/Resources/Mesh/MeshSchemeManager.cpp include/Core/GameTime.h include/Core/Gfx/StaticBuffer.h src/Resources/Mesh/MeshBuilder.cpp include/Core/Resources/Mesh/MeshBuilder.h include/Core/Resources/Mesh/MeshManager.h src/Resources/Mesh/MeshManager.cpp include/Core/Resources/Material/MaterialManager.h src/Resources/Material/MaterialManager.cpp include/Core/Gfx/CommandQueue.h src/Gfx/StaticBuffer.cpp src/Gfx/BufferManager.cpp include/Core/Gfx/BufferManager.h src/Gfx/Camera.cpp include/Core/Gfx/Camera.h src/Logic/RenderingComponent.cpp include/Core/Logic/RenderingComponent.h include/Core/Resources/Mesh/MeshScheme.h src/Resources/Shader/ShaderManager.cpp include/Core/Resources/Shader/ShaderManager.h include/Core/IManager.h src/IManager.cpp include/Core/MemoryModule.h src/MemoryModule.cpp src/WindowManager.cpp include/Core/WindowManager.h include/Core/Logic/System.h include/Core/Logic/World.h src/Logic/World.cpp include/Core/Logic/Entity.h include/Core/Logic/ISystem.h include/Core/Logic/EntityManager.h src/Logic/EntityManager.cpp src/Logic/ISystem.cpp src/Logic/Component.cpp include/Core/Resources/Texture/Texture.h src/Resources/Texture/Texture.cpp include/Core/Resources/Texture/TextureLoader.h src/Resources/Texture/TextureLoader.cpp include/Core/Resources/Texture/TextureManager.h src/Resources/Texture/TextureManager.cpp include/Core/Resources/Mesh/MeshLoader.h src/Resources/Mesh/MeshLoader.cpp include/Core/Input/ControlScheme.h include/Core/Input/InputManager.h src/Input/ControlScheme.cpp include/Core/Game.h src/Game.cpp src/Input/InputManager.cpp)
add_library(YAGE::Core ALIAS Core)

target_include_directories(Core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src )

target_compile_features(Core
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        cxx_nullptr)

target_link_libraries(Core YAGE::Utils ${SDL2_LIBRARY} glm assimp lodepng)

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
export(TARGETS ${ASSIMP_TARGETS} lodepng Core FILE CoreConfig.cmake)

if(YAGE_BUILD_TESTS)
    # Every library has unit tests, of course
    add_executable(CoreTests
            tests/CoreTests.cpp tests/MemoryModuleTests.cpp tests/WorldTests.cpp)

    target_link_libraries(CoreTests
            Core ${SDL2_LIBRARY} gtest gtest_main gmock)

    add_test(CoreTests CoreTests)
    add_coverage(Core)
endif(YAGE_BUILD_TESTS)