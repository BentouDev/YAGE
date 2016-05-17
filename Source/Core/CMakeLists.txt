## Provides basic classes, like meshes, shaders, materials
project(Core VERSION 0.1 LANGUAGES CXX)

set(VULKAN_API Source/Gfx/Vulkan/VulkanDevice.cpp Source/Gfx/Vulkan/VulkanDevice.h Source/Gfx/Vulkan/VulkanSwapchain.cpp Source/Gfx/Vulkan/VulkanSwapchain.h )
set(OPENGL_API Source/Gfx/OpenGl/OpenGlContext.cpp Source/Gfx/OpenGl/OpenGlContext.h )

if(VULKAN_FOUND)
    set(GRAPHICS_API ${VULKAN_API})
endif()

if(OPENGL_FOUND)
    set(GRAPHICS_API ${GRAPHICS_API} ${OPENGL_API} )
endif()

add_library(Core ${GRAPHICS_API} Source/Resources/Mesh.cpp Source/Resources/Mesh.h Source/Window.cpp Source/Window.h Source/Platform.h Source/Gfx/Viewport.cpp Source/Gfx/Viewport.h Source/InputManager.cpp Source/InputManager.h Source/Resources/Resource.h Source/Resources/ResourceManager.h ../Utils/include/Handle.h Source/Gfx/RenderTarget.cpp Source/Gfx/RenderTarget.h Source/Gfx/Context.cpp Source/Gfx/Context.h Source/Engine.cpp Source/Engine.h Source/Gfx/Rectangle.h Source/Gfx/Api/BaseDevice.h Source/Config.cpp Source/Config.h Source/Gfx/Renderer.cpp Source/Gfx/Renderer.h Source/Logger.h Source/Resources/Factory/MeshFactory.cpp Source/Resources/Factory/MeshFactory.h Source/Logic/Scene.cpp Source/Logic/Scene.h Source/Context.h Source/Console.cpp Source/Console.h Source/Context.cpp Source/Resources/Material.cpp Source/Resources/Material.h Source/Gfx/Buffer.cpp Source/Gfx/Buffer.h Source/Gfx/CommandQueue.cpp Source/Gfx/CommandQueue.h)
add_library(Volkhvy::Core ALIAS Core)

target_include_directories(Core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Utils/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE Source )

target_compile_features(Core
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        cxx_nullptr)

target_link_libraries(Core Volkhvy::Utils glm glfw)

if(VULKAN_FOUND)
    target_compile_definitions(Core PRIVATE VOLKHVY_VULKAN=1)
    target_include_directories(Core PRIVATE ${VULKAN_INCLUDE_DIR})
    target_link_libraries(Core ${VULKAN_LIBRARY})
endif()

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